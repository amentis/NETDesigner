#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QBoxLayout>
#include <QPainter>
#include <QPaintEvent>

#include "../NetGraph/netgraph.h"

#include "canvas.h"

class Canvas;

class Editor : public QWidget
{
    Q_OBJECT

private:
    Canvas* canvas;
    bool modified;
public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();
    bool isModified();
    bool save();
    void paint(QPainter* painter, QPaintEvent* event);

signals:

public slots:
};

#endif // EDITOR_H
