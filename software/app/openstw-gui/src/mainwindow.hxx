#pragma once

#include <QMainWindow>
#include <QMdiArea>
#include <QVBoxLayout>

#include "blinkgenerator.hxx"
#include "logsubwindow.hxx"
#include "simulationrunner.hxx"
#include "stelltischsubwindow.hxx"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Show given sub window, and make sure it is maximized, focussed
    // and on the top of all others.
    void showSubWindow(QMdiSubWindow* subWindow);

    // Hide given sub window.
    void hideSubWindow(QMdiSubWindow* subWindow);

protected slots:
    void simulationStarted();

    // Called when the main window is shown.
    void onWindowShown();

private slots:
    // == Menu entry handlers
    void on_actionExit_triggered();
    void on_actionLog_Messages_triggered(bool checked);

private:
    Ui::MainWindow* ui;
    QMdiArea* m_mdiArea{};
    SimulationRunner* m_simRunner{};
    StelltischSubWindow* m_stelltischWindow{};
    LogSubWindow* m_logWindow{};
    BlinkGenerator* m_blink;
};
