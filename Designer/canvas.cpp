#include "canvas.h"

#include <QDebug>

Canvas::Canvas(Editor *editorWidget, QWidget *parent) : QOpenGLWidget(parent), editor(editorWidget)
{
    setAutoFillBackground(true);
    setMouseTracking(true);
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
    editor->paint(&painter);
    painter.end();
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    editor->mouseMove(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    editor->mousePress(event);
}
