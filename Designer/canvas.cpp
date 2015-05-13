#include "canvas.h"

#include <QtGui>

#include "editor.h"

Canvas::Canvas(Editor *editorWidget, QWidget *parent) : QOpenGLWidget(parent), mEditor(editorWidget)
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

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(event->rect(), QBrush(QColor(255,255,255,255)));
    mEditor->paint(&painter);
    painter.end();
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    mEditor->mouseMove(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    mEditor->mousePress(event);
}
