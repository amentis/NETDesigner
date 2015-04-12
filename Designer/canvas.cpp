#include "canvas.h"

Canvas::Canvas(Editor *editorWidget, QWidget *parent) : QOpenGLWidget(parent), editor(editorWidget)
{
    setAutoFillBackground(true);
    addNodeDialog = nullptr;
}

Canvas::~Canvas()
{
    delete editor;
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
    if (event->button() == Qt::LeftButton){
        if (!addNodeDialog){
            AddNode addNodeDialog(new QPoint(QWidget::mapToGlobal(event->pos())) ,this);
            NetGraph::Node* node = addNodeDialog.run(dialogRect);
            if (node){
                node->setPosition(new QPoint(event->pos()));
                editor->addNode(node);
            }
        }
    }
    if (addNodeDialog && dialogRect){
        if (!dialogRect->contains(QWidget::mapToGlobal(event->pos()))) {
            addNodeDialog->close();
            delete addNodeDialog;
        }
    }
}

