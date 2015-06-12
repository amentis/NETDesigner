#include "programoptionsdialog.h"
#include "ui_programoptionsdialog.h"

#include <QtWidgets>
#include <Qt>

ProgramOptionsDialog::ProgramOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgramOptionsDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ProgramOptionsDialog::handleExit);
}

ProgramOptionsDialog::~ProgramOptionsDialog()
{
    delete ui;
}

ProgramOptionsDialog::ProgramOptionsDialog(int solutions, int loops, unsigned recursion, bool strategyBF, bool backtracking, unsigned maxPathLength, bool executeBack, bool oneArrow, QString* mainNetName, QStringList* nets, QWidget* parent) :
    QDialog(parent), ui(new Ui::ProgramOptionsDialog),
    mSolutions(solutions), mLoops(loops), mRrecursion(recursion), mStrategyBF(strategyBF), mBacktracking(backtracking),
    mMaxPathLength(maxPathLength), mExecuteBack(executeBack), mOneArrow(oneArrow), mMainNetName(mainNetName)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ProgramOptionsDialog::handleExit);

    QStringListModel* listModel = new QStringListModel(QStringList(nets->replaceInStrings("*", "")));
    ui->selectMainNetcomboBox->setModel(listModel);
    if (mainNetName)
        ui->selectMainNetcomboBox->setCurrentText(*mainNetName);
}

void ProgramOptionsDialog::getResults(int& solutions, int& loops, unsigned& recursion, bool& strategyBF, bool& backtracking, unsigned& maxPathLength, bool& executeBack, bool& oneArrow, QString*& mainNetName)
{
    solutions = mSolutions;
    loops = mLoops;
    recursion = mRrecursion;
    strategyBF = mStrategyBF;
    backtracking = mBacktracking;
    maxPathLength = mMaxPathLength;
    executeBack = mExecuteBack;
    oneArrow = mOneArrow;
    mainNetName = mMainNetName;
}

void ProgramOptionsDialog::handleExit(QAbstractButton* button)
{
    if (ui->buttonBox->standardButton(button) == ui->buttonBox->Ok){
        mMainNetName = new QString(ui->selectMainNetcomboBox->currentText());
        this->accept();
    } else if (ui->buttonBox->standardButton(button) == ui->buttonBox->Cancel){
        this->reject();
    }
}
