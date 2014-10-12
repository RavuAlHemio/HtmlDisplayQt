#include <cassert>

#include <QApplication>
#include <QCommandLineParser>

#include "MainWindow.h"

__attribute__((noreturn))
static void usageError(QCommandLineParser * parser, const QString &message)
{
    fprintf(stderr, "%s\n", qPrintable(message));
    parser->showHelp(1);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("HtmlDisplayQt");

    QCommandLineParser parser;
    parser.setApplicationDescription(QApplication::translate(
        "main",
        "Displays a web page, checking periodically whether it has changed and "
        "refreshing it whenever necessary."
    ));
    parser.addHelpOption();
    parser.addPositionalArgument(
        "url",
        QApplication::translate("main", "The URL to display.")
    );
    assert(parser.addOption(QCommandLineOption(
        QStringList() << "i" << "soft-interval",
        QApplication::translate(
            "main",
            "Interval in minutes between each check whether the page has "
            "updated and, potentially, its refresh."
        ),
        QApplication::translate("main", "minutes"),
        "5"
    )));
    assert(parser.addOption(QCommandLineOption(
        QStringList() << "I" << "hard-interval",
        QApplication::translate(
            "main",
            "Interval in minutes between each hard refresh."
        ),
        QApplication::translate("main", "minutes"),
        "60"
    )));
    assert(parser.addOption(QCommandLineOption(
        QStringList() << "F" << "no-fullscreen",
        QApplication::translate(
            "main",
            "Starts the application in windowed mode instead of fullscreen."
        )
    )));
    parser.process(app);

    const QStringList posArgs = parser.positionalArguments();
    if (posArgs.length() == 0)
    {
        usageError(&parser, QApplication::translate(
            "main",
            "Error: You must specify a URL."
        ));
    }
    else if (posArgs.length() > 1)
    {
        usageError(&parser, QApplication::translate(
            "main",
            "Error: Extraneous positional arguments."
        ));
    }

    QUrl url(posArgs.at(0));

    bool ok;

    QString softIntervalString = parser.value("soft-interval");
    unsigned long softInterval = softIntervalString.toULong(&ok);
    if (!ok || softInterval == 0)
    {
        usageError(&parser, QApplication::translate(
            "main",
            "Error: the soft interval must be a number greater than 0."
        ));
    }

    QString hardIntervalString = parser.value("hard-interval");
    unsigned long hardInterval = softIntervalString.toULong(&ok);
    if (!ok || hardInterval == 0)
    {
        usageError(&parser, QApplication::translate(
            "main",
            "Error: the hard interval must be a number greater than 0."
        ));
    }

    HtmlDisplayQt::MainWindow mw(url);
    mw.setSoftReloadInterval(softInterval);
    mw.setHardReloadInterval(hardInterval);
    mw.setFullScreen(!parser.isSet("no-fullscreen"));
    mw.show();

    return app.exec();
}
