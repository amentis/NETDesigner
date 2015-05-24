#include "editarrowdialog.h"
#include "ui_editarrowdialog.h"

#include <QtWidgets>

#include "base.h"
#include "arrow.h"

#include "primitive.h"

EditArrowDialog::EditArrowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditArrowDialog)
{
    ui->setupUi(this);
}

EditArrowDialog::EditArrowDialog(QVector<Base*>* bases, QStringList* nets, Arrow* arrow, QWidget* parent) :
    QDialog(parent), ui(new Ui::EditArrowDialog),
    mBases(bases), mNets(nets), mArrow(arrow)
{
    ui->setupUi(this);

    connect(ui->addButton, &QPushButton::clicked, this, &EditArrowDialog::addPrimitive);
    connect(ui->removeButton, &QPushButton::clicked, this, &EditArrowDialog::removePrimitive);
    connect(ui->upButton, &QPushButton::clicked, this, &EditArrowDialog::movePrimitiveUp);
    connect(ui->downButton, &QPushButton::clicked, this, &EditArrowDialog::movePrimitiveDown);
    connect(this, &EditArrowDialog::chainedPrimitivesListChanged, this, &EditArrowDialog::updateChainedPrimitivesList);

    QStringList available;

    for (const auto& base : *mBases){
        for (const auto& primitive : *(base->primitives()))
            available.append(*primitive->name());
    }

    QStringListModel* availableModel = new QStringListModel();
    availableModel->setStringList(available);
    ui->availablePrimitivesListView->setModel(availableModel);

    for (const auto& primitive : *(arrow->primitives())){
        chained.append(*(primitive->name()));
    }

    emit chainedPrimitivesListChanged();

    QStringListModel* netsModel = new QStringListModel(QStringList(*nets), this);
    ui->subnetComboBox->setModel(netsModel);
}

EditArrowDialog::~EditArrowDialog()
{
    delete ui;
}

void EditArrowDialog::applyResult()
{
    if (ui->callSubnetCheckBox->isChecked())
        mArrow->setCall(new QString(ui->subnetComboBox->currentText()));
    else
        mArrow->setCall();

    for (const auto& string : chained){
        QStringList strs = string.split("(", QString::SkipEmptyParts);
        QString primitiveName = strs[0].remove(" ");

        Primitive* primitive = nullptr;
        for (const auto& base : *mBases){
            for (const auto& pr : *(base->primitives())){
                if (pr->name() == primitiveName){
                    primitive = pr;
                    break;
                }
            }
            if (primitive)
                break;
        }

        mArrow->addPrimitive(primitive);
        QString argumentsStr = strs[1].remove(")").remove(" ");
        mArrow->setArgumentsForPrimitive(new QString(primitiveName), new QString(argumentsStr));
    }

}

void EditArrowDialog::addPrimitive()
{
    QString primitiveName = ui->availablePrimitivesListView->currentIndex().data().toString();


    Primitive* primitive = nullptr;

    for (Base* base : *mBases){
        for (Primitive* pr : *(base->primitives())){
            if (pr->name() == primitiveName){
                primitive = pr;
                break;
            }
        }
        if (primitive != nullptr)
            break;
    }
    if (primitive == nullptr)
        return;
    bool ok;
    QString text = QInputDialog::getText(this, tr("Primitive Arguments"),
                                         tr("Enter comma-separated arguments for primitive %1").
                                         arg(*(primitive->definition())),
                                         QLineEdit::Normal, "", &ok);
    if (!ok || text.isEmpty())
        return;
    chained.append(primitiveName + "(" + text + ")");
    emit chainedPrimitivesListChanged();
}

void EditArrowDialog::removePrimitive()
{
    chained.removeAll(ui->chainedPrimitivesListView->currentIndex().data().toString());
    emit chainedPrimitivesListChanged();
}

void EditArrowDialog::movePrimitiveUp()
{
    int currentRow = chained.indexOf(ui->chainedPrimitivesListView->currentIndex().data().toString());
    if (currentRow <= 0)
        return;
    QString item = chained.takeAt(currentRow);
    chained.insert(currentRow - 1, item);
    emit chainedPrimitivesListChanged();
}

void EditArrowDialog::movePrimitiveDown()
{
    int currentRow = chained.indexOf(ui->chainedPrimitivesListView->currentIndex().data().toString());
    if (currentRow < 0 || currentRow > chained.size() - 2)
        return;
    QString item = chained.takeAt(currentRow);
    chained.insert(currentRow + 1, item);
    emit chainedPrimitivesListChanged();
}

void EditArrowDialog::updateChainedPrimitivesList()
{
    QStringListModel* chainedModel = new QStringListModel();
    chainedModel->setStringList(chained);
    ui->chainedPrimitivesListView->setModel(chainedModel);
}
