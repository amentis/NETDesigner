#ifndef ARROW_H
#define ARROW_H

#include <QVector>

#include "node.h"
#include "../PrimitivesLoader/primitive.h"

class Node;

class Arrow
{
public:
    Arrow();
    ~Arrow();
private:
    QVector<Primitive> primitives;
    bool leadsToSubnet;
    Node* source;
    Node* destination;
};

#endif // ARROW_H
