#ifndef ADDNODE_H
#define ADDNODE_H

#include <QDialog>

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
    NetGraph::Node run();

private:
    Ui::AddNode *ui;
private slots:
    void checkExpressionField(int index);
};

#endif // ADDNODE_H
