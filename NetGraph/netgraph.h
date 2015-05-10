#ifndef NETGRAPH_H
#define NETGRAPH_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QString>
#include <QTextStream>
#include <QStringList>

#include "node.h"
#include "arrow.h"
#include "../PrimitivesLoader/primitive.h"

class NetGraph : public QObject
{
    Q_OBJECT

public:
    explicit NetGraph(QObject* parent = 0);
    ~NetGraph();

    void editNode(Node* target, Node::NodeType newType, QString* expression);
    void removeNode(Node *node);
    const QVector<Node *> *getNodes();

    void editArrow(QVector<Primitive*> added, QVector<Primitive*> removed);
    void removeArrow(Arrow* arrow);
    const QVector<Arrow*> *getArrows();

    bool hasStartNode();
    bool hasEndNode();

    void saveToStream(QTextStream& stream);
    bool loadFromStream(QTextStream& stream);

private:
    QVector<Node*>* nodes;
    QVector<Arrow*>* arrows;

    Node* start;
    Node* end;

public slots:
    void addNode(Node *newNode, QPoint* position);
    void addArrow(Node* from, Node* to, QString* expression = nullptr);

signals:
    void contentModified();

};

#endif // NETGRAPH_H
