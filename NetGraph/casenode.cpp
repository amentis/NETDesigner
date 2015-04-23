#include "casenode.h"


CaseNode::CaseNode(QString *expression)
{
    mType = NodeType::CaseNode;
    _expression = expression;
}

CaseNode::CaseNode(QString &expression) : CaseNode(new QString(expression)) {}

CaseNode::CaseNode(QString &&expression) : CaseNode(new QString(expression)) {}

CaseNode::~CaseNode()
{

}

void CaseNode::paint(QPainter *painter)
{

    QBrush outline(Qt::black);
    QPen out(outline, 1);

    QBrush fill(QColor(173, 214, 255));

    painter->setPen(out);
    painter->setBrush(fill);

    QPoint points[] = {QPoint(position->x() - 20, position->y() - 25),
                       QPoint(position->x() + 60, position->y() - 25),
                       QPoint(position->x() + 20, position->y() + 25),
                       QPoint(position->x() - 60, position->y() + 25)};

    painter->drawPolygon(points, 4);

    painter->drawText(QRect(position->x() - 50, position->y() - 20, 100, 40), Qt::AlignCenter, *_expression);
}
