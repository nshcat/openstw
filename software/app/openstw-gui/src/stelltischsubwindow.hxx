#pragma once

#include <QMdiSubWindow>
#include <QObject>
#include <QToolBar>

#include "magnettype.hxx"
#include "simulationrunner.hxx"
#include "tilepanelview.hxx"

class StelltischSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    StelltischSubWindow(QWidget* parent, SimulationRunner* sim);

protected:
    void setupToolbar();

protected slots:
    // Emitted by SimulationRunner after the simulation has fully initialized.
    // It is safe to initialize tile renderers at that point.
    void simulationStarted();
    void onAddMagnetTriggered(const MagnetType type);

protected:
    TilePanelView* m_tilePanelView{};
    SimulationRunner* m_simulation{}; //< Owned by main window
    QToolBar* m_toolBar{};
};
