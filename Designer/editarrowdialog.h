#ifndef EDITARROWDIALOG_H
#define EDITARROWDIALOG_H

#include <QDialog>
template<typename T>class QVector;
class Arrow;
class Base;
class QStringList;

namespace Ui {
class EditArrowDialog;
}

class EditArrowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditArrowDialog(QWidget *parent = 0);
    EditArrowDialog(QVector<Base*>* bases, QStringList* nets, Arrow* arrow, QWidget* parent = 0);
    ~EditArrowDialog();
    void applyResult();
private:
    Ui::EditArrowDialog *ui;

    QVector<Base*>* mBases;
    QStringList* mNets;
    Arrow* mArrow;

    QStringList chained;

private slots:
    void addPrimitive();
    void removePrimitive();
    void movePrimitiveUp();
    void movePrimitiveDown();

    void updateChainedPrimitivesList();

signals:
    void chainedPrimitivesListChanged();
};

#endif // EDITARROWDIALOG_H
