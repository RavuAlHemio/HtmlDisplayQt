#include <QBoxLayout>
#include <QMainWindow>
#include <QTimer>
#include <QWebEngineView>

namespace HtmlDisplayQt
{

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(unsigned int softReloadInterval READ softReloadInterval WRITE setSoftReloadInterval NOTIFY softReloadIntervalChanged)
    Q_PROPERTY(unsigned int hardReloadInterval READ hardReloadInterval WRITE setHardReloadInterval NOTIFY hardReloadIntervalChanged)
    Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged)

private:
    QUrl m_url;
    QByteArray m_lastPageBytes;
    unsigned long m_softReloadInterval;
    unsigned long m_hardReloadInterval;
    bool m_fullScreen;

    unsigned long m_timerCounter;

    QBoxLayout *m_layout;
    QNetworkAccessManager *m_netMan;
    QWidget *m_container;
    QWebEngineView *m_webView;
    QTimer *m_minuteTimer;

public slots:
    void minuteTimerTick();
    void networkAccessFinished(QNetworkReply * reply);
    void performSoftReload();
    void performHardReload();

public:
    MainWindow(const QUrl & url, QWidget * parent = 0);
    ~MainWindow();

public:
    unsigned int softReloadInterval() const
    {
        return m_softReloadInterval;
    }
    void setSoftReloadInterval(unsigned int newInterval)
    {
        if (newInterval == 0 || m_softReloadInterval == newInterval)
        {
            return;
        }
        m_softReloadInterval = newInterval;
        emit softReloadIntervalChanged(m_softReloadInterval);
    }

    unsigned int hardReloadInterval() const
    {
        return m_hardReloadInterval;
    }
    void setHardReloadInterval(unsigned int newInterval)
    {
        if (newInterval == 0 || m_hardReloadInterval == newInterval)
        {
            return;
        }
        m_hardReloadInterval = newInterval;
        emit hardReloadIntervalChanged(m_hardReloadInterval);
    }

    bool fullScreen() const
    {
        return m_fullScreen;
    }
    void setFullScreen(bool newValue);

signals:
    void softReloadIntervalChanged(unsigned int);
    void hardReloadIntervalChanged(unsigned int);
    void fullScreenChanged(bool);
};

}
