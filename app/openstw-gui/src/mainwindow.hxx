#pragma once

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

private:
    Ui::MainWindow* ui;
    QVBoxLayout* m_layout{};
    TilePanelView* m_tilePanelView{};
    Openstw::Simulation::Simulation m_simulation;
};
