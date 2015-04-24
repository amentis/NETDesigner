#include "addnode.h"
#include "ui_addnode.h"

AddNode::AddNode(QWidget *parent) :
    QDialog(parent), ui(new Ui::AddNode)
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
}

Node *AddNode::getResult()
{
    Node::NodeType type;
    switch (ui->comboBox->currentIndex()){
            case 0: type = Node::NodeType::OrdinaryNode; break;
            case 1: type = Node::NodeType::CaseNode; break;
            case 2: type = Node::NodeType::ProximityNode; break;
            case 3: type = Node::NodeType::StartNode; break;
            case 4: type = Node::NodeType::EndNode; break;
            }
    return new Node(type, ui->expression->text());
}

void AddNode::close()
{
    ui->comboBox->setCurrentIndex(0);
    ui->expression->clear();
    this->hide();
}


void AddNode::checkExpressionField(int index)
{
    if (index != 1 && index != 2){
        ui->expression->hide();
    } else {
        ui->expression->show();
    }
}
