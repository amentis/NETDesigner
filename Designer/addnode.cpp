#include "addnode.h"
#include "ui_addnode.h"

AddNode::AddNode(QPoint *pos, QWidget *parent) :
    QDialog(parent), ui(new Ui::AddNode), position(pos)
{
    ui->setupUi(this);

    connect(ui->comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &AddNode::checkExpressionField);
    connect(ui->submit, &QPushButton::clicked, this, &AddNode::submit);
}

AddNode::~AddNode()
{
    delete ui;

    delete position;
}

NetGraph::Node *AddNode::run(QRect *rect)
{
    NetGraph::Node* toReturn = nullptr;
    this->move(*position);
    this->setWindowFlags(Qt::Popup);
    rect = new QRect(this->rect());
    int result = this->exec();
    if (result == QDialog::Accepted){
        switch (resultType){
        case 0: toReturn = (NetGraph::Node*) (new NetGraph::OrdinaryNode()); break;
        case 1: toReturn = (NetGraph::Node*) (new NetGraph::CaseNode(resultExpression->toStdString())); break;
        case 2: toReturn = (NetGraph::Node*) (new NetGraph::ProximityNode(resultExpression->toStdString())); break;
        case 4: toReturn = (NetGraph::Node*) (new NetGraph::StartNode()); break;
        case 5: toReturn = (NetGraph::Node*) (new NetGraph::EndNode()); break;
        }
    }
    return toReturn;
}

void AddNode::close()
{
    this->setResult(QDialog::Rejected);
}

void AddNode::checkExpressionField(int index)
{
    if (index == 0){
        ui->expression->setVisible(false);
    } else {
        ui->expression->setVisible(true);
    }
}

void AddNode::submit()
{
    resultType = ui->comboBox->currentIndex();
    if (resultType == 0) {
        resultExpression = nullptr;
    } else {
        resultExpression = new QString(ui->expression->text());
    }
    this->setResult(QDialog::Accepted);
}
