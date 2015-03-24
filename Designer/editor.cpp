#include "editor.h"

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    canvas = new Canvas(this, this);
    layout->addWidget(canvas);
    setLayout(layout);
    modified = false;
}

Editor::~Editor()
{

}

bool Editor::isModified()
{
    return modified;
}

bool Editor::save()
{
    //implement save, is ok = true
    return true;
}

void Editor::paint(QPainter *painter, QPaintEvent *event)
{
}

