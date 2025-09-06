#include "application.hxx"
#include "mainwindow.hxx"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    createApplication();
    MainWindow w;
    w.show();
    return a.exec();
}
