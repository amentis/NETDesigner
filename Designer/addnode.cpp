#include "addnode.h"
#include "ui_addnode.h"

AddNode::AddNode(QWidget *parent) :
    QDialog(parent), ui(new Ui::AddNode), result(nullptr)
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
    switch (ui->comboBox->currentIndex()){
            case 0: result = (Node*) (new OrdinaryNode()); break;
            case 1: result = (Node*) (new CaseNode(ui->expression->text())); break;
            case 2: result = (Node*) (new ProximityNode(ui->expression->text())); break;
            case 3: result = (Node*) (new StartNode()); break;
            case 4: result = (Node*) (new EndNode()); break;
            }
    return result;
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
