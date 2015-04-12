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

void Editor::addNode(NetGraph::Node *node)
{
    nodes->append(node);
}

void Editor::paint(QPainter *painter, QPaintEvent *event)
{

}
