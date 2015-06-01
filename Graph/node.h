#ifndef NODE_H
#define NODE_H

#include "graph_global.h"

#include <QObject>
template<typename T>class QVector;
class QString;
class QPoint;
class QRect;
class QPainter;
class Arrow;

class GRAPHSHARED_EXPORT Node : public QObject
{

    Q_OBJECT

public:
    enum class NodeType {OrdinaryNode, ProximityNode, CaseNode, StartNode, EndNode};
    explicit Node(QObject* parent = 0);
    Node(NodeType type, QString* expression, QObject* parent = 0);
    Node(NodeType type, QString&& expression, QObject* parent = 0);
    ~Node();
    void setPosition(QPoint* pos);
    QPoint *position();

    void paint(QPainter *painter);

    void setType(NodeType newType);
    void setExpression(QString* newExpression);
    QString *expression();

    NodeType type() const;
    QRect* rect();
    QRect* tightRect();

    void removeArrowIn(Arrow* arrow) const;
    void removeArrowOut(Arrow* arrow) const;
    void addArrowIn(Arrow* newArrow);
    void addArrowOut(Arrow* newArrow);

    const QVector<Arrow*>* arrowsIn();
    const QVector<Arrow*>* arrowsOut();

private:
    void setRect();
    void setTightRect();

    void paintStartNode(QPainter *painter);
    void paintEndNode(QPainter *painter);
    void paintOrdinaryNode(QPainter *painter);
    void paintCaseNode(QPainter *painter);
    void paintProximityNode(QPainter *painter);

    NodeType mType;
    QString* mExpression;
    QPoint* mPosition;
    QVector<Arrow*> *mArrowsIn;
    QVector<Arrow*> *mArrowsOut;
    QRect* mRect; // for placing arrowButton
    QRect* mTightRect; // for placing arrows
};

#endif // NODE_H
