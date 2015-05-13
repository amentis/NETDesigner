#ifndef ADDARROWEXPRESSIONDIALOG_H
#define ADDARROWEXPRESSIONDIALOG_H

#include <QDialog>

class QString;

namespace Ui {
class AddArrowExpressionDialog;
}

class AddArrowExpressionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddArrowExpressionDialog(bool isCase, QWidget *parent = 0);
    ~AddArrowExpressionDialog();

    QString* expression();

private:
    Ui::AddArrowExpressionDialog *ui;
};

#endif // ADDARROWEXPRESSIONDIALOG_H
