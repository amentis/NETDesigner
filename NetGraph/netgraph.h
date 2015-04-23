#ifndef NETGRAPH_H
#define NETGRAPH_H

#include <QVector>

#include "startnode.h"
#include "endnode.h"
#include "node.h"
#include "ordinarynode.h"
#include "proximitynode.h"
#include "casenode.h"

class NetGraph
{
private:
    QVector<Node*>* nodes;
    QVector<Arrow*>* arrows;

    StartNode* start;
    EndNode* end;

public:
    NetGraph();
    ~NetGraph();

    void addNode(Node *newNode);
    const QVector<Node *> *getNodes();

    bool hasStartNode();
    bool hasEndNode();
};

#endif // NETGRAPH_H
