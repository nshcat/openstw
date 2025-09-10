#include "stelltischsubwindow.hxx"
#include "rendering/magnetgraphicsobject.hxx"

#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>
#include <functional>

StelltischSubWindow::StelltischSubWindow(QWidget* parent, SimulationRunner* sim)
    : QMdiSubWindow{parent, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint}, m_simulation{sim}
{
    this->setWindowTitle("Stelltisch");
    this->resize(1768, 770);

    this->m_toolBar = new QToolBar();
    this->setupToolbar();

    this->m_tilePanelView = new TilePanelView();

    auto* layout = new QVBoxLayout();
    layout->setMenuBar(this->m_toolBar);
    layout->addWidget(this->m_tilePanelView);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* layoutContent = new QWidget();
    layoutContent->setLayout(layout);
    this->setWidget(layoutContent);

    connect(this->m_simulation, &SimulationRunner::simulationStarted, this, &StelltischSubWindow::simulationStarted);
}

void StelltischSubWindow::setupToolbar()
{
    // == Magnet drop down button
    QToolButton* magnetButton = new QToolButton(this);
    magnetButton->setText("Add magnet..");
    magnetButton->setPopupMode(QToolButton::InstantPopup);

    QMenu* magnetMenu = new QMenu(magnetButton);
    QAction* addTrackClosedAction = magnetMenu->addAction("Track closed");
    connect(addTrackClosedAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::TrackClosed));

    QAction* addCatenaryPowerlessAction = magnetMenu->addAction("Caternary powerless");
    connect(addCatenaryPowerlessAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::CatenaryPowerless));

    QAction* addCounterTractAction = magnetMenu->addAction("Counter track usage");
    connect(addCounterTractAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::CounterTrackInUse));

    QAction* addRpAction = magnetMenu->addAction("Räumungsprüfung required");
    connect(addRpAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::RaeumungsPruefung));

    QAction* addApAction = magnetMenu->addAction("Abschnittsprüfung required");
    connect(addApAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::AbschnittsPruefung));

    QAction* addKlAction = magnetMenu->addAction("Kleinwagen present");
    connect(addKlAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::Kleinwagen));

    QAction* addLueAction = magnetMenu->addAction("Lue-train present");
    connect(addLueAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::LueTrain));

    QAction* addNlfAction = magnetMenu->addAction("No counter track usage");
    connect(addNlfAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::NoCounterTrackUse));

    QAction* addArbAction = magnetMenu->addAction("Work in progress");
    connect(addArbAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::WorkInProgress));

    QAction* addBueAction = magnetMenu->addAction("Level crossing malfunction");
    connect(addBueAction, &QAction::triggered, this,
            std::bind(&StelltischSubWindow::onAddMagnetTriggered, this, MagnetType::LevelCrossingMalfunction));

    magnetButton->setMenu(magnetMenu);

    this->m_toolBar->addWidget(magnetButton);
    // ==
}

void StelltischSubWindow::simulationStarted()
{
    this->m_tilePanelView->setTilePanel(this->m_simulation->simulation()->tilePanel());
}

void StelltischSubWindow::onAddMagnetTriggered(const MagnetType type)
{
    auto* magnetObject = new Rendering::MagnetGraphicsObject(type);
    this->m_tilePanelView->scene()->addItem(magnetObject);
    magnetObject->setPos(50, 50);
}
