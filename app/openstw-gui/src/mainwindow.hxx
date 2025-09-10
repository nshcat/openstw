#pragma once

#include "simulationrunner.hxx"
#include "stelltischsubwindow.hxx"
#include <QMainWindow>
#include <QMdiArea>
#include <QVBoxLayout>
#include <simulation.hxx>

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

protected slots:
    void simulationStarted();

private slots:
    void on_actionExit_triggered();

private:
    Ui::MainWindow* ui;
    QMdiArea* m_mdiArea{};
    SimulationRunner* m_simRunner{};
    StelltischSubWindow* m_stelltischWindow{};
};
