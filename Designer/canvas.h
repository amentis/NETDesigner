#ifndef CANVAS_H
#define CANVAS_H

#include <QOpenGLWidget>
#include <QPaintEvent>
#include <QPainter>

#include "editor.h"

class Editor;

class Canvas : public QOpenGLWidget
{

    Q_OBJECT
public:
    Canvas(Editor* editorWidget, QWidget * parent = 0);
    ~Canvas();
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;

private:
    Editor* editor;
};

#endif // CANVAS_H
