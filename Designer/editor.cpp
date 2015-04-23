#include "editor.h"

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    canvas = new Canvas(this, this);
    layout->addWidget(canvas);
    setLayout(layout);
    modified = false;

    netGraph = new NetGraph();

    addNodeDialog = new AddNode(this);
    connect(addNodeDialog, &AddNode::accepted, this, &Editor::addNode);
    addNodeDialogOpened = false;

    nodePosition = nullptr;
}

Editor::~Editor()
{
    delete canvas;
}

bool Editor::isModified()
{
    return modified;
}

bool Editor::save()
{
    //TODO: implement save, is ok = true
    return true;
}

void Editor::addNode()
{
    Node* node = addNodeDialog->getResult();
    node->setPosition(nodePosition);
    netGraph->addNode(node);
    canvas->repaint();
}



void Editor::paint(QPainter *painter)
{
    for (const auto& node : *netGraph->getNodes()){
        switch (node->type()) {
        case Node::NodeType::StartNode: (dynamic_cast<StartNode*>(node))->paint(painter); break;
        case Node::NodeType::EndNode: (dynamic_cast<EndNode*>(node))->paint(painter); break;
        case Node::NodeType::OrdinaryNode: (dynamic_cast<OrdinaryNode*>(node))->paint(painter); break;
        case Node::NodeType::CaseNode: (dynamic_cast<CaseNode*>(node))->paint(painter); break;
        case Node::NodeType::ProximityNode: (dynamic_cast<ProximityNode*>(node))->paint(painter); break;
        }
    }
}

void Editor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        if (!addNodeDialogOpened){
            addNodeDialog->move(QWidget::mapToGlobal(event->pos()));
            nodePosition = new QPoint(QWidget::mapToParent(event->pos()));
            addNodeDialog->show();
            addNodeDialogOpened = true;
        } else {
            addNodeDialog->close();
            addNodeDialogOpened = false;
        }
    }
}
