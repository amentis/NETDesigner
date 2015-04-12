#ifndef ADDNODE_H
#define ADDNODE_H

#include <QDialog>
#include <QPoint>

#include "../NetGraph/node.h"
#include "../NetGraph/ordinarynode.h"
#include "../NetGraph/casenode.h"
#include "../NetGraph/proximitynode.h"
#include "../NetGraph/systemnodes.h"

namespace Ui
{
class AddNode;
}

class AddNode : public QDialog
{
    Q_OBJECT

public:
    explicit AddNode(QPoint* pos, QWidget *parent = 0);
    ~AddNode();
    NetGraph::Node *run(QRect* rect);
    void close();

private:
    Ui::AddNode *ui;
    QPoint* position;

    unsigned short resultType;
    QString* resultExpression;

private slots:
    void checkExpressionField(int index);
    void submit();
};

#endif // ADDNODE_H
