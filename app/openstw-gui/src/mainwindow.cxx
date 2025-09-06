#include "mainwindow.hxx"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_tilePanelView = new TilePanelView();

    this->m_layout = new QVBoxLayout();
    this->m_layout->addWidget(this->m_tilePanelView);
    this->ui->centralwidget->setLayout(this->m_layout);

    this->m_simRunner = new SimulationRunner();
    connect(this->m_simRunner, &SimulationRunner::simulationStarted, this, &MainWindow::simulationStarted);
    this->m_simRunner->startSimulation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::simulationStarted()
{
    this->m_tilePanelView->setTilePanel(this->m_simRunner->simulation()->tilePanel());
}
