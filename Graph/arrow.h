#ifndef ARROW_H
#define ARROW_H

#include "graph_global.h"

#include <QObject>
#include <QVector>

class QPoint;
class QPainter;

class Primitive;

class Node;

class GRAPHSHARED_EXPORT Arrow : public QObject
{

    Q_OBJECT

public:
    explicit Arrow(QObject* parent = 0);
    Arrow(Node* from, Node* to, QString* expression = nullptr, QObject* parent = 0);
    ~Arrow();

    void paint(QPainter* painter);

    bool contains(const QPoint &point);

    Node* from();
    Node* to();
    QString* expression();
    const QVector<Primitive*>* primitives();

private:
    QVector<Primitive*>* mPrimitives;
    bool leadsToSubnet;
    Node* mFrom;
    Node* mTo;
    QString* mExpression;

    void calculatePathsAndRect();

    QVector<QRect>* mRects;
    QPoint* drawPath;
    unsigned drawPathLength;
    QPoint* drawHead;
    QPoint* labelPosition;

};

#endif // ARROW_H
