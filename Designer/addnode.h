#ifndef ADDNODE_H
#define ADDNODE_H

#include <QDialog>
#include <QString>
#include <QPoint>
#include <QMouseEvent>

#include "../NetGraph/node.h"

namespace Ui
{
class AddNode;
}

class AddNode : public QDialog
{
    Q_OBJECT

public:
    explicit AddNode(QWidget *parent = 0);
    ~AddNode();
    Node * getResult();

private:
    Ui::AddNode *ui;

    unsigned short resultType;
    QString* resultExpression;

public slots:
    void close();

private slots:
    void checkExpressionField(int index);
};


#endif // ADDNODE_H
