#ifndef NETGRAPH_H
#define NETGRAPH_H

#include <QObject>
#include <QVector>
#include <QPoint>

#include "startnode.h"
#include "endnode.h"
#include "node.h"
#include "ordinarynode.h"
#include "proximitynode.h"
#include "casenode.h"

class NetGraph : public QObject
{
    Q_OBJECT

public:
    explicit NetGraph(QObject* parent = 0);
    ~NetGraph();

    void addNode(Node *newNode, QPoint* position);
    void removeNode(Node *node);
    const QVector<Node *> *getNodes();

    bool hasStartNode();
    bool hasEndNode();

private:
    QVector<Node*>* nodes;
    QVector<Arrow*>* arrows;

    StartNode* start;
    EndNode* end;

signals:
    void contentModified();

};

#endif // NETGRAPH_H
