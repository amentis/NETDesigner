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
private:
    QVector<Primitive*> *mPrimitives;
    bool leadsToSubnet;
    Node* mFrom;
    Node* mTo;
    QString* mExpression;
};

#endif // ARROW_H
