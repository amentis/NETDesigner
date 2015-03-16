#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->showMaximized();

  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));
  connect(qApp, SIGNAL(aboutToQuit()), this,SLOT(exit()));

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::createProject()
{

}

void MainWindow::openProject()
{

}

void MainWindow::closeProject()
{

}

void MainWindow::saveProject()
{

}

void MainWindow::exit()
{

}

void MainWindow::addNet()
{

}

void MainWindow::removeNet()
{

}

void MainWindow::saveAllNets()
{

}

void MainWindow::saveNet()
{

}

void MainWindow::browsePrimitives()
{

}

void MainWindow::build()
{

}

void MainWindow::run()
{

}

void MainWindow::debug()
{

}
