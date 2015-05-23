#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QModelIndex>

class QString;
class QStringListModel;
class QStringList;
class QCloseEvent;

class Base;

#include "editor.h"

class BrowsePrimitivesDialog;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setBases(QVector<Base*>* newBases);

private:
    Ui::MainWindow *ui;
    QString* projectDirectory;
    QString* mainNetName;
    QVector<Base*>* projectBases;
    QStringListModel* netsListModel;
    QStringList netsList;

    BrowsePrimitivesDialog* browsePrimitivesDialog;

    bool mModified;

    void saveNet(Editor* editor);
    int tabIndex;

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void createProject();
    void openProject();
    void closeProject();
    void saveProject();
    void exit();

    void addNet();
    void openNet(QModelIndex index);
    void closeNet(int index);
    void removeNet();
    void saveAllNets();
    void saveNet();
    void browsePrimitives();

    void build();
    void run();
    void debug();

    void enableOrDisableRemoveNet();

    void projectLoad();
    void projectUnload();

    void help();
    void about();

    void modified();
    void saved();

    void checkTabModified(int index);
signals:
    void projectLoaded();
    void projectUnloaded();
    void netCountModified();
};

#endif // MAINWINDOW_H
