#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
};

#endif // MAINWINDOW_H
