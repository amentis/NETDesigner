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

NetGraph::Node *AddNode::getResult()
{
    switch (ui->comboBox->currentIndex()){
            case 0: result = (NetGraph::Node*) (new NetGraph::OrdinaryNode()); break;
            case 1: result = (NetGraph::Node*) (new NetGraph::CaseNode(ui->expression->text().toStdString())); break;
            case 2: result = (NetGraph::Node*) (new NetGraph::ProximityNode(ui->expression->text().toStdString())); break;
            case 4: result = (NetGraph::Node*) (new NetGraph::StartNode()); break;
            case 5: result = (NetGraph::Node*) (new NetGraph::EndNode()); break;
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
    if (index == 0){
        ui->expression->hide();
    } else {
        ui->expression->show();
    }
}
