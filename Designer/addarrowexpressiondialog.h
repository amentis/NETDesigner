#ifndef ADDARROWEXPRESSIONDIALOG_H
#define ADDARROWEXPRESSIONDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class AddArrowExpressionDialog;
}

class AddArrowExpressionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddArrowExpressionDialog(bool isCase, QWidget *parent = 0);
    ~AddArrowExpressionDialog();

    const QString expression();

private:
    Ui::AddArrowExpressionDialog *ui;
};

#endif // ADDARROWEXPRESSIONDIALOG_H
