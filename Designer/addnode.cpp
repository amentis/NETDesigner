#include "addnode.h"
#include "ui_addnode.h"

#include <QtGui>

AddNode::AddNode(QWidget *parent) :
    QDialog(parent), ui(new Ui::AddNode), mResultType(0), mResultExpression(nullptr), mEditMode(false), mEditable(nullptr)
{
    ui->setupUi(this);
    connect(ui->comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &AddNode::checkExpressionField);
    connect(ui->submit, &QPushButton::clicked, this, &AddNode::accept);
    this->setWindowFlags(Qt::Popup);
    ui->expression->hide();
}

AddNode::~AddNode()
{
    delete ui;
    delete mResultExpression;
}

void AddNode::getResult(Node *&node, Node::NodeType& type, QString*& expression)
{
    switch (ui->comboBox->currentIndex()){
            case 0: type = Node::NodeType::OrdinaryNode; break;
            case 1: type = Node::NodeType::CaseNode; break;
            case 2: type = Node::NodeType::ProximityNode; break;
            case 3: type = Node::NodeType::StartNode; break;
            case 4: type = Node::NodeType::EndNode; break;
            }

    if (mEditMode){
        node = mEditable;
        expression = new QString(ui->expression->text());
    } else {
        node = new Node(type, ui->expression->text());
    }
}

void AddNode::getResult(Node*& node)
{
    QString* tmp;
    Node::NodeType type;
    getResult(node, type, tmp);
}


void AddNode::showEditMode(Node *node)
{
    mEditMode = true;

    mEditable = node;

    switch (node->type()) {
    case Node::NodeType::OrdinaryNode: ui->comboBox->setCurrentIndex(0);break;
        case Node::NodeType::CaseNode: ui->comboBox->setCurrentIndex(1);break;
        case Node::NodeType::ProximityNode: ui->comboBox->setCurrentIndex(2);break;
        case Node::NodeType::StartNode: ui->comboBox->setCurrentIndex(3);break;
        case Node::NodeType::EndNode: ui->comboBox->setCurrentIndex(4);break;
    }
    ui->expression->setText(*node->expression());

    this->show();
}

void AddNode::showAddMode()
{
    mEditMode = false;

    ui->comboBox->setCurrentIndex(0);
    ui->expression->clear();

    this->show();
}

bool AddNode::editMode()
{
    return mEditMode;
}

void AddNode::checkExpressionField(int index)
{
    if (index != 1 && index != 2){
        ui->expression->hide();
    } else {
        ui->expression->show();
    }
}
