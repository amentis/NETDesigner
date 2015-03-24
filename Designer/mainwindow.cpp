#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->showMaximized();

  connect(ui->actionNew_Project, SIGNAL(triggered()), this, SLOT(createProject()));
  connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));
  connect(ui->actionClose_Project, SIGNAL(triggered()), this, SLOT(closeProject()));
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

  connect(qApp, SIGNAL(aboutToQuit()), this,SLOT(exit()));

  connect(ui->actionAdd_Net, SIGNAL(triggered()), this, SLOT(addNet()));
  connect(ui->actionRemove_Net, SIGNAL(triggered()), this, SLOT(removeNet()));
  connect(ui->actionSave_Current_Net, SIGNAL(triggered()), this, SLOT(saveNet()));
  connect(ui->actionSave_All_Nets, SIGNAL(triggered()), this, SLOT(saveAllNets()));
  connect(ui->actionBrowse_Primitives_Bases, SIGNAL(triggered()), this, SLOT(browsePrimitives()));

  connect(ui->actionBuild, SIGNAL(triggered()), this, SLOT(build()));
  connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(run()));
  connect(ui->actionDebug, SIGNAL(triggered()), this, SLOT(debug()));

  connect(this, SIGNAL(projectLoaded()), this, SLOT(projectLoad()));
  connect(this, SIGNAL(projectUnloaded()), this, SLOT(projectUnload()));

  connect(ui->netsListView, SIGNAL(clicked(QModelIndex)), this, SLOT(openNet(QModelIndex)));
  connect(ui->editorTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeNet(int)));

  connect(this, SIGNAL(netCountModified()), this, SLOT(enableOrDisableRemoveNet()));

  projectDirectory = NULL;
  projectBases = NULL;

  netsListModel = new QStringListModel(this);
  netsListModel->setStringList(netsList);
  ui->netsListView->setModel(netsListModel);

}

MainWindow::~MainWindow()
{
    delete netsListModel;
    delete ui;
}

void MainWindow::createProject()
{

  if (projectDirectory){
    closeProject();
    if (projectDirectory)
      return;
  }

  QFileDialog dialog(
        this, tr("Create New Project"),
        QDir::homePath(),
        tr("NETDesigner Project Directories"));
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setFileMode(QFileDialog::Directory);

  if (dialog.exec() == QDialog::Accepted){

    QDir dir = dialog.selectedFiles()[0];

    if (dir.exists()){
      if (!dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0){
        if (!QFile(QString(dir.absolutePath() + "/net.project")).exists()){
          QErrorMessage msg;
          msg.showMessage(tr("Invalid directory!"));
          return;
        }
      }
    } else {
      dir.mkpath(".");
      if (!dir.exists()){
        QErrorMessage msg;
        msg.showMessage(tr("Cannot create given directory!"));
        return;
      }
    }
    QFile file(QString(dir.absolutePath() + "/net.project"));
    file.open(QFile::WriteOnly | QIODevice::Text);
    file.close();
    if (!file.exists()){
      QErrorMessage msg;
      msg.showMessage(tr("Cannot write in given directory!"));
      return;
    }
    projectDirectory = new QString(dir.absolutePath());
    projectBases = new QVector<PrimitivesLoader::Base*>();
    emit projectLoad();
  }
}

void MainWindow::openProject()
{
  if (projectDirectory){
    closeProject();
    if (projectDirectory)
      return;
  }

  QString path = QFileDialog::getOpenFileName(this, tr("Open Project"),
                                             QDir::homePath(),
                                             tr("NETDesigner Projects (net.project)"));
  if (path.isEmpty())
    return;

  QFile projectFile(path);

  if (!projectFile.open(QFile::ReadOnly | QIODevice::Text)){
    QErrorMessage msg;
    msg.showMessage(tr("Cannot read from project file!"));
    return;
  }

  QTextStream stream(&projectFile);

  mainNetName = stream.readLine();

  projectBases = new QVector<PrimitivesLoader::Base*>();

  while (!stream.atEnd()){
    projectBases->append(new PrimitivesLoader::Base(stream.readLine().toStdString()));
  }

  projectFile.close();

  path.remove("/net.project");
  projectDirectory = new QString(path);

  QStringList filters;

  filters << "*.net";
  QDir projectDir(*projectDirectory);
  QStringList files = projectDir.entryList(filters, QDir::Files);

  for (int i = 0; i < files.size(); ++i) {
      QString tmp = files.at(i);
      tmp.remove(".net");
      netsList.append(tmp);
  }

  emit netCountModified();

  emit projectLoaded();

}

void MainWindow::closeProject()
{
  if (ui->actionSave_Project->isEnabled()){
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(
          this, tr("NETDesigner"), tr("You have unsaved data! Do you want to save before you quit?"),
          QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if (confirm == QMessageBox::Yes){
      saveProject();
      if (ui->actionSave_Project->isEnabled())
        return;
    } else if (confirm == QMessageBox::Cancel){
      return;
    }
  }

  netsList.clear();

  emit netCountModified();

  emit projectUnloaded();
}

void MainWindow::saveProject()
{
  QFile projectFile(QString(*projectDirectory + "net.project"));

  projectFile.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text);

  QTextStream stream(&projectFile);

  stream << mainNetName;

  for (int i = 0; i < projectBases->size(); ++i) {
    stream << QString(projectBases->at(i)->getName().c_str());
  }

  projectFile.close();

  saveAllNets();
  ui->actionSave_Project->setEnabled(false);
}

void MainWindow::exit()
{
  closeProject();
  qApp->exit(0);
}

void MainWindow::addNet()
{
    QString name = QInputDialog::getText(this, "Add Net", "Net name:");

    if (name.isEmpty() || name.contains(QRegularExpression("~`!@#$%^&*()_=+[{]}\\|\"\'<>/?")) ||
            !name.at(0).isLetter()){
        QErrorMessage msg;
        msg.showMessage(tr("Invalid Net Name!"));
        return;
    }

    QFile net(QString(*projectDirectory + "/" + name + ".net"));

    net.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    net.close();

    if (!net.exists()){
        QErrorMessage msg;
        msg.showMessage(tr("Could not write net!"));
        return;
    }

    netsList.append(name);

    emit netCountModified();
}

void MainWindow::openNet(QModelIndex index)
{

    for (int i = 0; i < ui->editorTabWidget->count(); ++i) {
        if (ui->editorTabWidget->tabText(i) == QString("&" + index.data().toString()))
        {
            ui->editorTabWidget->setCurrentIndex(i);
            return;
        }
    }

    QString netName = index.data().toString();

    Editor* editor = new Editor(ui->editorTabWidget);

    int newTabIndex = ui->editorTabWidget->addTab(editor, netName);
    ui->editorTabWidget->setCurrentIndex(newTabIndex);

    emit netCountModified();
}

void MainWindow::closeNet(int index)
{
    if (((Editor*)ui->editorTabWidget->widget(index))->isModified()){
        if (!((Editor*)ui->editorTabWidget->widget(index))->save())
            return;
    }
    ui->editorTabWidget->removeTab(index);

    emit netCountModified();
}

void MainWindow::removeNet()
{
      QMessageBox::StandardButton confirm;
      confirm = QMessageBox::question(
            this, tr("NETDesigner"), tr("Are you sure you want to delete this net?"),
            QMessageBox::Yes|QMessageBox::No);
      QString netName = QString(ui->editorTabWidget->tabText(ui->editorTabWidget->currentIndex())).remove("&");
      if (confirm == QMessageBox::Yes){
        QFile file(QString(*projectDirectory + "/" + netName + ".net"));
        if (!file.remove()){
            QErrorMessage msg;
            msg.showMessage(tr("Cannot delete net!"));
            return;
        }
      } else if (confirm == QMessageBox::No){
        return;
      }
      netsList.removeAll(netName);
      closeNet(ui->editorTabWidget->currentIndex());

      emit netCountModified();
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

void MainWindow::enableOrDisableRemoveNet()
{
    if (ui->editorTabWidget->count() > 0){
        ui->actionRemove_Net->setEnabled(true);
    } else {
        ui->actionRemove_Net->setEnabled(false);
    }

    netsListModel->setStringList(netsList);
}

void MainWindow::projectLoad()
{
    ui->actionClose_Project->setEnabled(true);

    ui->menuProject->setEnabled(true);
    ui->actionAdd_Net->setEnabled(true);
    ui->actionBrowse_Primitives_Bases->setEnabled(true);

    ui->menuBuild_Run->setEnabled(true);
    ui->actionBuild->setEnabled(true);
    ui->actionRun->setEnabled(true);
    ui->actionDebug->setEnabled(true);
}

void MainWindow::projectUnload()
{
    delete projectDirectory;
    projectDirectory = NULL;
    delete projectBases;
    projectBases = NULL;
    ui->actionClose_Project->setEnabled(false);
    ui->menuProject->setEnabled(false);
    ui->actionAdd_Net->setEnabled(false);
    ui->actionBrowse_Primitives_Bases->setEnabled(false);

    ui->menuBuild_Run->setEnabled(false);
    ui->actionBuild->setEnabled(false);
    ui->actionRun->setEnabled(false);
    ui->actionDebug->setEnabled(false);
}
