#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
template<typename T>class QVector;
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
    QVector<Base*>* projectBases;
    QStringListModel* netsListModel;
    QStringList netsList;

    BrowsePrimitivesDialog* browsePrimitivesDialog;

    bool mModified;

    void saveNet(Editor* editor);
    int tabIndex;

    //options
    int solutions; // -1 ask after all 0 all
    int loops; // -1 No 0 Any
    unsigned recursion; // 0 Any
    bool strategyBF; //true - bf false - df
    bool backtracking;
    unsigned maxPathLength; // 0 - Any
    bool executeBack;
    bool oneArrow;
    QString* mainNetName;

    bool lastBuildSuccessful; //used for chaining build and run

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
    void programOptions();

    void build();
    void run();
    void debug();
    void clean();

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
