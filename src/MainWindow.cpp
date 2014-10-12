#include "MainWindow.h"

#include <QMetaEnum>
#include <QNetworkReply>

namespace HtmlDisplayQt
{

MainWindow::MainWindow(const QUrl & url, QWidget * parent)
    : QMainWindow(parent), m_url(url), m_lastPageBytes()
    , m_softReloadInterval(5), m_hardReloadInterval(60), m_fullScreen(true)
    , m_timerCounter(0)
{
    m_container = new QWidget();

    m_layout = new QBoxLayout(QBoxLayout::TopToBottom);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_webView = new QWebEngineView();
    m_webView->setContent(QByteArray());

    m_netMan = new QNetworkAccessManager(this);
    this->connect(
        m_netMan, SIGNAL(finished(QNetworkReply *)),
        this, SLOT(networkAccessFinished(QNetworkReply *))
    );

    m_minuteTimer = new QTimer(this);
    m_minuteTimer->setSingleShot(false);
    this->connect(
        m_minuteTimer, SIGNAL(timeout()),
        this, SLOT(minuteTimerTick())
    );
    m_minuteTimer->start(60000);

    m_errorLabel = new QLabel("", this);
    m_errorLabel->setAlignment(Qt::AlignRight);
    m_errorLabel->setIndent(4);
    m_errorLabel->setVisible(false);
    QFont labelFont = m_errorLabel->font();
    labelFont.setPointSize(16);
    m_errorLabel->setFont(labelFont);

    m_layout->addWidget(m_webView);
    m_layout->addWidget(m_errorLabel);

    m_container->setLayout(m_layout);

    this->setCentralWidget(m_container);
    this->showFullScreen();

    this->performHardReload();
}

void MainWindow::minuteTimerTick()
{
    ++m_timerCounter;

    qDebug() << "Tick.";

    if (m_timerCounter % m_hardReloadInterval == 0)
    {
        qDebug() << "Hard reload!";
        this->performHardReload();
    }
    else if (m_timerCounter % m_softReloadInterval == 0)
    {
        qDebug() << "Soft reload!";
        this->performSoftReload();
    }
}

void
MainWindow::performSoftReload()
{
    m_netMan->get(QNetworkRequest(m_url));
}

void
MainWindow::performHardReload()
{
    m_lastPageBytes.clear();
    this->performSoftReload();
}

void
MainWindow::networkAccessFinished(QNetworkReply * reply)
{
    qDebug() << "Network access finished!";

    if (reply->error() == QNetworkReply::NoError)
    {
        m_errorLabel->setText(QString());
        m_errorLabel->hide();
    }
    else
    {
        // find the enumeration value's string representation
        const QMetaObject & mo = QNetworkReply::staticMetaObject;
        QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("NetworkError"));
        QString errorMessage = QString("%1 (%2)").arg(me.valueToKey(reply->error())).arg(reply->error());

        m_errorLabel->setText(errorMessage);
        m_errorLabel->show();
    }

    QByteArray currentBytes = reply->readAll();
    if (m_lastPageBytes == currentBytes)
    {
        qDebug() << "Page content is unchanged.";
        return;
    }

    qDebug() << "Page content has changed! Refreshing.";

    // refresh the page!
    m_lastPageBytes = currentBytes;
    m_webView->setUrl(m_url);
}

void
MainWindow::setFullScreen(bool newValue)
{
    if (m_fullScreen == newValue)
    {
        return;
    }

    m_fullScreen = newValue;
    if (m_fullScreen)
    {
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
    }
    emit fullScreenChanged(m_fullScreen);
}

MainWindow::~MainWindow()
{
}

}
