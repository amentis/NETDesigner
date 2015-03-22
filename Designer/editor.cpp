#include "editor.h"

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    QBoxLayout* layout = new QBoxLayout(this);
    canvas = new QOpenGLWidget(this);
    layout->addWidget(canvas);
    setLayout(layout);
}

Editor::~Editor()
{

}

