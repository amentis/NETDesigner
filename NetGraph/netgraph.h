#ifndef NETGRAPH_H
#define NETGRAPH_H

#include <QObject>
#include <QVector>
#include <QPoint>

#include "node.h"

class NetGraph : public QObject
{
    Q_OBJECT

public:
    explicit NetGraph(QObject* parent = 0);
    ~NetGraph();

    void addNode(Node *newNode, QPoint* position);
    void editNode(Node* target, Node::NodeType newType, QString* expression);
    void removeNode(Node *node);
    const QVector<Node *> *getNodes();

    bool hasStartNode();
    bool hasEndNode();

private:
    QVector<Node*>* nodes;
    QVector<Arrow*>* arrows;

    Node* start;
    Node* end;

signals:
    void contentModified();

};

#endif // NETGRAPH_H
