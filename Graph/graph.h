#ifndef GRAPH_H
#define GRAPH_H

#include "graph_global.h"

#include <QObject>
#include <QVector>

class QPoint;
class QString;
class QTextStream;

class Primitive;

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

    void editArrow(QVector<Primitive*> added, QVector<Primitive*> removed);
    void removeArrow(Arrow* arrow);
    const QVector<Arrow*>* arrows();

    bool hasStartNode();
    bool hasEndNode();

    void saveToStream(QTextStream& stream);
    bool loadFromStream(QTextStream& stream);

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
