#include "editor.h"

void Editor::operateAddNodeDialog(QMouseEvent *event)
{
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

void Editor::operateDeleteNodeDialog(Node *selected)
{
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, tr("NETDesigner"), tr("Are you sure you want to delete this node?"),
                      QMessageBox::Yes | QMessageBox::No);
    if (confirm == QMessageBox::Yes){
        netGraph->removeNode(selected);
    }
}

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    canvas = new Canvas(this, this);
    layout->addWidget(canvas);
    setLayout(layout);
    modified = false;

    netGraph = new NetGraph(this);

    connect(netGraph, &NetGraph::contentModified, canvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));

    addNodeDialog = new AddNode(this);
    connect(addNodeDialog, &AddNode::accepted, this, &Editor::addNode);
    addNodeDialogOpened = false;

    arrowButton = new ArrowButton();

    connect(arrowButton, &ArrowButton::visibleChanged, canvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));
    connect(arrowButton, &ArrowButton::hoveredChanged, canvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));

    nodePosition = nullptr;

}

Editor::~Editor()
{
    delete canvas;
    delete arrowButton;
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
    netGraph->addNode(node, nodePosition);
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
    arrowButton->paint(painter);
}

void Editor::mousePress(QMouseEvent *event)
{
    for (const auto& node : *netGraph->getNodes()) {
        if (node->rect()->contains(QWidget::mapToParent(event->pos()))){
            if (event->button() == Qt::RightButton){
                operateDeleteNodeDialog(node);
            }
            return;
        }
    }
    if (event->button() == Qt::LeftButton){
        operateAddNodeDialog(event);
        return;
    }
}

void Editor::mouseMove(QMouseEvent *event)
{
    Node* selected = nullptr;
    for (const auto& node : *netGraph->getNodes()) {
        if (node->rect()->contains(QWidget::mapToParent(event->pos()))){
            selected = node;
            break;
        }
    }
    if (selected){
        arrowButton->setVisible(true, selected);
        if (arrowButton->rect()->contains(QWidget::mapToParent(event->pos()))){
            arrowButton->setHovered(true);
        } else {
            arrowButton->setHovered(false);
        }
    } else {
        arrowButton->setVisible(false, nullptr);
    }
}
