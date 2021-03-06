#ifndef GRAPH_H
#define GRAPH_H

#include "graph_global.h"

#include <QObject>
template<typename T>class QVector;
class QPoint;
class QString;
class QTextStream;

class Primitive;
class Base;

class Arrow;

#include "node.h"

class GRAPHSHARED_EXPORT Graph : public QObject
{
    Q_OBJECT

public:
    explicit Graph(QObject* parent = 0);
    ~Graph();

    void editNode(Node* target, Node::NodeType newType, QString* expression);
    void removeNode(Node *node);
    const QVector<Node*>* nodes();

    void removeArrow(Arrow* arrow);
    const QVector<Arrow*>* arrows();

    bool hasStartNode();
    Node* startNode();
    bool hasEndNode();
    Node* endNode();

    void saveToStream(QTextStream& stream);
    bool loadFromStream(QTextStream& stream, QVector<Base*>* bases);
    unsigned internalNodeIndex(Node* node);

private:

    QVector<Node*>* mNodes;
    QVector<Arrow*>* mArrows;

    Node* mStart;
    Node* mEnd;

public slots:
    void addNode(Node* newNode, QPoint* position);
    void addArrow(Node* from, Node* to, QString* expression = nullptr);

signals:
    void contentModified();

};

#endif // GRAPH_H
