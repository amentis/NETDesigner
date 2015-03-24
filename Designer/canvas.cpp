#include "canvas.h"



Canvas::Canvas(Editor *editorWidget, QWidget *parent) : QOpenGLWidget(parent)
{
    editor = editorWidget;
    setAutoFillBackground(true);
}

Canvas::~Canvas()
{

}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(QColor(255,255,255,255)));
    editor->paint(&painter, event);
    painter.end();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{

}

