#include <QApplication>

#include "application.hxx"
#include "logging.hxx"
#include "logmanager.hxx"
#include "mainwindow.hxx"

// The global logging manager.
// All log messages emitted by the Qt logging infrastructure
// will end up in this object.
LogManager* g_LogManager{nullptr};

// Message handler that catches log messages emitted by the Qt logging
// infrastructure.
void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // Ignore all log messages until the global log manager
    // object has been constructed.
    if (!g_LogManager)
    {
        return;
    }

    // Make sure to make a copy of the category string - since it will get deallocated
    // as soon as this function returns!
    QString category = context.category ? QString{context.category} : QString{"default"};

    // Ignore log messages from the default category.
    // This filters out lots of annoying Qt messages caused by limitations
    // imposed by Wayland..
    if (category == "default")
    {
        return;
    }

    // Marshal the call into the Ui thread, since g_LogManager acts as
    // a list item model that the Ui directly accesses - and log messages
    // can originate from any thread!
    // clang-format off
    QMetaObject::invokeMethod(g_LogManager,
        [type, category, msg]()
        {
            if(g_LogManager)
            {
                g_LogManager->addMessage(type, category, msg);
            }
        }
        , Qt::QueuedConnection);
    // clang-format on
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // Setup global log manager, and route all log messages
    // to it via a custom message handler.
    g_LogManager = new LogManager(&a);
    qInstallMessageHandler(messageHandler);

    qCInfo(catGui, "Starting application");

    createApplication();
    MainWindow w;
    w.show();
    return a.exec();
}
