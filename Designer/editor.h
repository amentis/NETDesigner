#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVector>
#include <QPoint>

#include "../NetGraph/netgraph.h"
#include "../NetGraph/node.h"
#include "../NetGraph/arrow.h"

#include "canvas.h"
#include "addnode.h"

class Canvas;

class Editor : public QWidget
{
    Q_OBJECT

private:
    Canvas* canvas;

    QVector<NetGraph::Node*>* nodes;
    QVector<NetGraph::Arrow*>* arrows;

    AddNode* addNodeDialog;
    bool addNodeDialogOpened;

    QPoint* nodePosition;

    bool modified;
public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();
    bool isModified();
    bool save();

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void paint(QPainter* painter, QPaintEvent* event);

signals:

public slots:
    void addNode();
};

#endif // EDITOR_H
