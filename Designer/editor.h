#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVector>

#include "../NetGraph/netgraph.h"
#include "../NetGraph/node.h"
#include "../NetGraph/arrow.h"

#include "canvas.h"

class Canvas;

class Editor : public QWidget
{
    Q_OBJECT

private:
    Canvas* canvas;

    QVector<NetGraph::Node*>* nodes;
    QVector<NetGraph::Arrow*>* arrows;

    bool modified;
public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();
    bool isModified();
    bool save();

    void addNode(NetGraph::Node* node);

    void paint(QPainter* painter, QPaintEvent* event);

signals:

public slots:
};

#endif // EDITOR_H
