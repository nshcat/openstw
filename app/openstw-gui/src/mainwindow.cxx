#include "mainwindow.hxx"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("OpenStw Simulation");

    this->m_mdiArea = new QMdiArea();
    this->setCentralWidget(this->m_mdiArea);

    this->m_simRunner = new SimulationRunner();
    connect(this->m_simRunner, &SimulationRunner::simulationStarted, this, &MainWindow::simulationStarted);

    this->resize(1920, 1080);

    this->m_stelltischWindow = new StelltischSubWindow(nullptr, this->m_simRunner);
    this->m_mdiArea->addSubWindow(this->m_stelltischWindow);

    this->m_simRunner->startSimulation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::simulationStarted()
{
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
