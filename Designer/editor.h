#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QBoxLayout>

class Canvas;
#include "NetGraph/node.h"
#include "NetGraph/startnode.h"
#include "NetGraph/casenode.h"
#include "NetGraph/endnode.h"
#include "NetGraph/ordinarynode.h"
#include "NetGraph/proximitynode.h"
#include "NetGraph/arrow.h"
#include "NetGraph/netgraph.h"

#include "canvas.h"
#include "addnode.h"

class Editor : public QWidget
{
    Q_OBJECT

private:
    Canvas* canvas;

    NetGraph* netGraph;

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
    void paint(QPainter* painter);

signals:

public slots:
    void addNode();
};

#endif // EDITOR_H
