#include "editor.h"

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    canvas = new Canvas(this, this);
    layout->addWidget(canvas);
    setLayout(layout);
    modified = false;

    nodes = new QVector<NetGraph::Node*>;
    arrows = new QVector<NetGraph::Arrow*>;

    addNodeDialog = new AddNode(this);
    connect(addNodeDialog, &AddNode::accepted, this, &Editor::addNode);
    addNodeDialogOpened = false;

    nodePosition = nullptr;
}

Editor::~Editor()
{
    delete canvas;
    delete nodes;
    delete arrows;
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
    NetGraph::Node * node = addNodeDialog->getResult();
    node->setPosition(nodePosition);
    nodes->append(node);
}



void Editor::paint(QPainter *painter, QPaintEvent *event)
{

}

void Editor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        if (!addNodeDialogOpened){
            addNodeDialog->move(QWidget::mapToGlobal(event->pos()));
            nodePosition = new QPoint(event->pos());
            addNodeDialog->show();
            addNodeDialogOpened = true;
        } else {
            addNodeDialog->close();
            addNodeDialogOpened = false;
        }
    }
}
