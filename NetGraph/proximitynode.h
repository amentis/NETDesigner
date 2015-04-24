#ifndef PROXIMITYNODE_H
#define PROXIMITYNODE_H

#include <QString>
#include <QVector>

#include "node.h"

class ProximityNode : public Node
{
public:
    ProximityNode(QString *expression);
    ProximityNode(QString & expression);
    ProximityNode(QString && expression);
    ~ProximityNode();

    void setRect() Q_DECL_OVERRIDE;
    void paint(QPainter *painter) Q_DECL_OVERRIDE;
private:
    QString* nodeExpression;
    QVector<QString> arrowExpressions;
};

#endif // PROXIMITYNODE_H
