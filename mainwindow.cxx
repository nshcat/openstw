#include "mainwindow.hxx"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), m_tilePanel{8, 4}
{
    ui->setupUi(this);

    this->m_tilePanelView = new TilePanelView();

    this->m_layout = new QVBoxLayout();
    this->m_layout->addWidget(this->m_tilePanelView);
    this->ui->centralwidget->setLayout(this->m_layout);

    // XXX pass actual tile panel instance ptr
    this->m_tilePanelView->setTilePanel(&this->m_tilePanel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
