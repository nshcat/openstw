#include "mainwindow.hxx"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      m_simulation{Openstw::Simulation::Simulation::FromTemplateFile("sim_template.xml")}
{
    ui->setupUi(this);

    this->m_tilePanelView = new TilePanelView();

    this->m_layout = new QVBoxLayout();
    this->m_layout->addWidget(this->m_tilePanelView);
    this->ui->centralwidget->setLayout(this->m_layout);
    this->m_tilePanelView->setTilePanel(this->m_simulation.tilePanel());
}

MainWindow::~MainWindow()
{
    delete ui;
}
