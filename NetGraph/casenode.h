#ifndef CASENODE_H
#define CASENODE_H

#include <QString>
#include <QVector>

#include "node.h"

class CaseNode : public Node
{
public:
    CaseNode(QString *expression);
    CaseNode(QString& expression);
    CaseNode(QString&& expression);
    ~CaseNode();

    void paint(QPainter *painter) Q_DECL_OVERRIDE;
private:
    QString* _expression;
    QVector<int> arrowConstants;
};

#endif // CASENODE_H
