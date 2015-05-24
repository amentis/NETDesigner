#ifndef ARROW_H
#define ARROW_H

#include "graph_global.h"

#include <QObject>
#include <QVector>
#include <QHash>

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
    void setOptions(int loops, unsigned recursion, bool executeBack);
    void options(int& loops, unsigned& recursion, bool& executeBack);
    void setCall(QString* subNet = nullptr);
    void addPrimitive(Primitive* primitive);
    QString* argumentsForPrimitive(QString* primitiveName);
    void setArgumentsForPrimitive(QString* primitiveName, QString* arguments);

    bool subnetCalled(QString*& subnetName);

private:
    QVector<Primitive*>* mPrimitives;
    QHash<QString, QString*> mPrimitiveArguments;
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
    QPoint* primitivesPosition;

    QString* callTo;

    //options
    int mLoops; //0 any -1 no
    unsigned mRecursion; //0 any
    bool mExecuteBack;

};

#endif // ARROW_H
