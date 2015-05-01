#include "addarrowexpressiondialog.h"
#include "ui_addarrowexpressiondialog.h"

AddArrowExpressionDialog::AddArrowExpressionDialog(bool isCase, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArrowExpressionDialog)
{
    ui->setupUi(this);

    if (isCase){ //otherwise is proximity
        ui->label->setText(tr("Arrow constant:"));
        setWindowTitle(tr("Set Arrow constant:"));
    }
}

AddArrowExpressionDialog::~AddArrowExpressionDialog()
{
    delete ui;
}

const QString AddArrowExpressionDialog::expression()
{
    return ui->inputField->text();
}
