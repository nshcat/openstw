#pragma once

#include "simulationrunner.hxx"
#include "tilepanelview.hxx"
#include <QMainWindow>
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

private:
    Ui::MainWindow* ui;
    QVBoxLayout* m_layout{};
    TilePanelView* m_tilePanelView{};
    SimulationRunner* m_simRunner{};
};
