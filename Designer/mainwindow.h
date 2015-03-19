#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QFile>
#include <QErrorMessage>
#include <QMessageBox>
#include <QVector>
#include <QIODevice>
#include <QTextStream>
#include <QInputDialog>

#include "../PrimitivesLoader/base.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  QString* projectDirectory;
  QString mainNetName;
  QVector<PrimitivesLoader::Base*>* projectBases;

private slots:
  void createProject();
  void openProject();
  void closeProject();
  void saveProject();
  void exit();

  void addNet();
  void removeNet();
  void saveAllNets();
  void saveNet();
  void browsePrimitives();

  void build();
  void run();
  void debug();

  void projectLoad();
  void projectUnload();
signals:
  void projectLoaded();
  void projectUnloaded();
};

#endif // MAINWINDOW_H
