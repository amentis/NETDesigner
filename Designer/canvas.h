#ifndef CANVAS_H
#define CANVAS_H

#include <QOpenGLWidget>

class QWidget;
class QPaintEvent;
class QMouseEvent;

class Editor;

class Canvas : public QOpenGLWidget
{

    Q_OBJECT

public:
    explicit Canvas(Editor* editorWidget, QWidget* parent = 0);
    ~Canvas();
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private:
    Editor* mEditor;
};

#endif // CANVAS_H
