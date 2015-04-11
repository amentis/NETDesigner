#include "addnode.h"
#include "ui_addnode.h"

AddNode::AddNode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNode)
{
    ui->setupUi(this);

    connect(ui->comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &AddNode::checkExpressionField);
}

AddNode::~AddNode()
{
    delete ui;
}

NetGraph::Node AddNode::run()
{

}

void AddNode::checkExpressionField(int index)
{
    if (index == 0){
        ui->expression->setVisible(false);
    } else {
        ui->expression->setVisible(true);
    }
}
