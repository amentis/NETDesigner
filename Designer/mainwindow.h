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
#include <QRegularExpression>
#include <QStringListModel>
#include <QStringList>
#include <QModelIndex>
#include <QVariant>
#include <QTextStream>

#include "../PrimitivesLoader/base.h"

#include "editor.h"

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

private:
    Ui::MainWindow *ui;
    QString* projectDirectory;
    QString mainNetName;
    QVector<Base*>* projectBases;
    QStringListModel* netsListModel;
    QStringList netsList;

    bool mModified;

    void saveNet(Editor* editor);
    int tabIndex;

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
