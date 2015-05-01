#ifndef ARROW_H
#define ARROW_H

#include <QVector>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPoint>
#include <math.h>

#include "node.h"
#include "../PrimitivesLoader/primitive.h"

class Node;

class Arrow
{
public:
    Arrow();
    Arrow(Node* from, Node* to, QString* expression = nullptr);
    ~Arrow();

    void paint(QPainter * painter);

    bool intersects(QPoint* point);
private:
    QVector<Primitive*> *mPrimitives;
    bool leadsToSubnet;
    Node* mFrom;
    Node* mTo;
    QString* mExpression;

    void calculatePathsAndRect();

    QVector<QRect>* rects;
    QPoint* drawPath;
    unsigned drawPathLength;
    QPoint* drawHead;
    QPoint* labelPosition;

};

#endif // ARROW_H
