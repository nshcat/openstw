#include "mainwindow.hxx"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("OpenStw Simulation");

    this->m_mdiArea = new QMdiArea();
    this->setCentralWidget(this->m_mdiArea);

    this->m_blink = new BlinkGenerator{this};

    this->m_simRunner = new SimulationRunner();
    connect(this->m_simRunner, &SimulationRunner::simulationStarted, this, &MainWindow::simulationStarted);

    this->resize(1920, 1080);

    // == Create and add sub windows
    // = Stelltisch sub window
    this->m_stelltischWindow = new StelltischSubWindow(nullptr, this->m_simRunner, this->m_blink);
    this->m_mdiArea->addSubWindow(this->m_stelltischWindow);
    // =

    // = Logging sub window
    this->m_logWindow = new LogSubWindow{nullptr};
    this->m_mdiArea->addSubWindow(this->m_logWindow);

    // Do not destroy the sub window object upon closing, just hide it.
    this->m_logWindow->setAttribute(Qt::WA_DeleteOnClose, false);

    // Make sure that the View->Windows->Log Messages checkbox is always
    // synced with the log subwindows visibility state, even if the user closes
    // the subwindow with the close button.
    // clang-format off
    connect(this->m_logWindow, &SubWindow::visibilityChanged, ui->actionLog_Messages, &QAction::setChecked);
    // clang-format on
    // =

    // Defer the positioning of sub windows to after the main window is shown,
    // because otherwise, the MDI area viewport size is not set yet.
    QTimer::singleShot(0, this, &MainWindow::onWindowShown);
    // ==

    this->m_simRunner->startSimulation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSubWindow(QMdiSubWindow* subWindow)
{
    // Make sure the inner widget is not hidden, this sometimes
    // happens if the other hides the sub window by pressing X.
    if (subWindow->widget())
    {
        subWindow->widget()->show();
    }

    // Maximize sub window, in case the user had minimized it before
    // hiding it.
    if (subWindow->isMinimized())
    {
        subWindow->showNormal();
    }
    else
    {
        subWindow->show();
    }

    // Bring the sub window to the front..
    subWindow->raise();

    // And give it focus.
    subWindow->setFocus();

    // Finally, set it as the active sub window.
    this->m_mdiArea->setActiveSubWindow(subWindow);
}

void MainWindow::hideSubWindow(QMdiSubWindow* subWindow)
{
    // Hide it.
    subWindow->hide();
}

void MainWindow::simulationStarted()
{
    // Start the blink timer.
    this->m_blink->start();
}

void MainWindow::onWindowShown()
{
    // == Position sub windows
    const auto mdiViewportArea = this->m_mdiArea->viewport()->rect();

    // Place the log window at the bottom left of the MDI area.
    this->m_logWindow->move(0, mdiViewportArea.height() - this->m_logWindow->height());
    // ==
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionLog_Messages_triggered(bool checked)
{
    if (checked)
    {
        this->showSubWindow(this->m_logWindow);
    }
    else
    {
        this->hideSubWindow(this->m_logWindow);
    }
}
