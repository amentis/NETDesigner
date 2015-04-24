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
    void getResult(Node *&node, Node::NodeType& type, QString *&expression);
    void getResult(Node *&node);

    void showEditMode(Node* node);
    void showAddMode();

    bool isEditMode();

private:
    Ui::AddNode *ui;

    unsigned short resultType;
    QString* resultExpression;

    bool mEditMode;
    Node* editable;

private slots:
    void checkExpressionField(int index);
};


#endif // ADDNODE_H
