#ifndef ADDNODE_H
#define ADDNODE_H

#include <QDialog>

#include "node.h"

namespace Ui
{
class AddNode;
}

class QString;

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

    bool editMode();

private:
    Ui::AddNode *ui;

    unsigned short mResultType;
    QString* mResultExpression;

    bool mEditMode;
    Node* mEditable;

private slots:
    void checkExpressionField(int index);
};


#endif // ADDNODE_H
