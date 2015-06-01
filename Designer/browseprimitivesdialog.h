#ifndef BROWSEPRIMITIVESDIALOG_H
#define BROWSEPRIMITIVESDIALOG_H

#include <QDialog>
class QStringList;
class QStringListModel;
class QAbstractButton;
class QString;

class PrimitivesLoader;
class Base;

class MainWindow;

namespace Ui {
class BrowsePrimitivesDialog;
}

class BrowsePrimitivesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrowsePrimitivesDialog(QWidget *parent = 0);
    BrowsePrimitivesDialog(QVector<Base*>*& bases, QString* projectDirectory, QWidget * parent = 0);
    ~BrowsePrimitivesDialog();

private:
    Ui::BrowsePrimitivesDialog *ui;
    QVector<Base*>* mBases;
    QString* projectPrimitivesPath;

    PrimitivesLoader* primitivesLoader;

    QStringListModel* basesListModel;
    QStringList* basesList;
    QStringListModel* primitivesListModel;
    QStringList* primitivesList;

    MainWindow* mParent;

    void setUnloadEnabled(bool enabled);

private slots:
    void loadBase();
    void unloadBase();
    void setBasesList();
    void setPrimitivesList(const QModelIndex& index);
    void handleButtonBox(QAbstractButton * button);

signals:
    void basesListChanged();
};

#endif // BROWSEPRIMITIVESDIALOG_H
