#include "arrow.h"

Arrow::Arrow() :
    mPrimitives(new QVector<Primitive*>()), leadsToSubnet(false), mFrom(nullptr), mTo (nullptr), mExpression(nullptr) {}

Arrow::Arrow(Node *from, Node *to, QString *expression) :
mPrimitives(new QVector<Primitive*>), leadsToSubnet(false), mFrom(from), mTo(to), mExpression(expression) {
    mFrom->addArrowOut(this);
    mTo->addArrowIn(this);
}

Arrow::~Arrow()
{
    delete mPrimitives;
    delete mExpression;
}

void Arrow::paint(QPainter *painter)
{
    QBrush brush(Qt::black);
    QPen pen(brush, 1);
    painter->setPen(pen);

    bool fromIsLeftFromTo;
    bool fromIsHigherThanTo;
    bool targetIsSide;

    fromIsLeftFromTo =  (mFrom->tightRect()->x() < mTo->tightRect()->x());

    fromIsHigherThanTo = (mFrom->tightRect()->center().y() < mTo->tightRect()->center().y());

    int xDistance = abs(mFrom->tightRect()->center().x() - mTo->tightRect()->center().x());
    int yDistance = abs(mFrom->tightRect()->center().y() - mTo->tightRect()->center().y());

    //draw arrow body

    if (abs(xDistance - yDistance) < 50){
        //side of from, top/bottom of to
        targetIsSide = false;
        painter->drawLine((fromIsLeftFromTo)? mFrom->tightRect()->right() : mFrom->tightRect()->left(),
                          mFrom->tightRect()->center().y(),
                          mTo->tightRect()->center().x(),
                          mFrom->tightRect()->center().y());
        painter->drawLine(mTo->tightRect()->center().x(),
                          mFrom->tightRect()->center().y(),
                          mTo->tightRect()->center().x(),
                          (mFrom->tightRect()->center().y() < mTo->tightRect()->center().y())?
                              mTo->tightRect()->top() : mTo->tightRect()->bottom());

    } else if (xDistance < yDistance){
        //top to bottom
        targetIsSide = false;
        if (fromIsHigherThanTo){
            painter->drawLine(mFrom->tightRect()->center().x(),
                              mFrom->tightRect()->bottom(),
                              mFrom->tightRect()->center().x(),
                              mTo->tightRect()->top() + (mFrom->tightRect()->bottom() - mTo->tightRect()->top())/2);
            painter->drawLine(mFrom->tightRect()->center().x(),
                              mTo->tightRect()->top() + (mFrom->tightRect()->bottom() - mTo->tightRect()->top())/2,
                              mTo->tightRect()->center().x(),
                              mTo->tightRect()->top() + (mFrom->tightRect()->bottom() - mTo->tightRect()->top())/2);
            painter->drawLine(mTo->tightRect()->center().x(),
                              mTo->tightRect()->top() + (mFrom->tightRect()->bottom() - mTo->tightRect()->top())/2,
                              mTo->tightRect()->center().x(),
                              mTo->tightRect()->top());
        } else {
            painter->drawLine(mFrom->tightRect()->center().x(),
                              mFrom->tightRect()->top(),
                              mFrom->tightRect()->center().x(),
                              mTo->tightRect()->bottom() + (mFrom->tightRect()->top() - mTo->tightRect()->bottom())/2);
            painter->drawLine(mFrom->tightRect()->center().x(),
                              mTo->tightRect()->bottom() + (mFrom->tightRect()->top() - mTo->tightRect()->bottom())/2,
                              mTo->tightRect()->center().x(),
                              mTo->tightRect()->bottom() + (mFrom->tightRect()->top() - mTo->tightRect()->bottom())/2);
            painter->drawLine(mTo->tightRect()->center().x(),
                              mTo->tightRect()->bottom() + (mFrom->tightRect()->top() - mTo->tightRect()->bottom())/2,
                              mTo->tightRect()->center().x(),
                              mTo->tightRect()->bottom());
        }
    } else {
        //side of from, side of to
        targetIsSide = true;
        painter->drawLine((fromIsLeftFromTo)? mFrom->tightRect()->right() : mFrom->tightRect()->left(),
                          mFrom->tightRect()->center().y(),
                          mFrom->tightRect()->right() + (mTo->tightRect()->left() - mFrom->tightRect()->right())/2,
                          mFrom->tightRect()->center().y());
        painter->drawLine(mFrom->tightRect()->right() + (mTo->tightRect()->left() - mFrom->tightRect()->right())/2,
                          mFrom->tightRect()->center().y(),
                          mFrom->tightRect()->right() + (mTo->tightRect()->left() - mFrom->tightRect()->right())/2,
                          mTo->tightRect()->center().y());
        painter->drawLine(mFrom->tightRect()->right() + (mTo->tightRect()->left() - mFrom->tightRect()->right())/2,
                          mTo->tightRect()->center().y(),
                          (fromIsLeftFromTo)? mTo->tightRect()->left() : mTo->tightRect()->right(),
                          mTo->tightRect()->center().y());
    }

    //draw arrow head

    painter->setBrush(brush);
    if (targetIsSide){
        if (fromIsLeftFromTo){
            QPoint points[] = {QPoint(mTo->tightRect()->left() - 6, mTo->tightRect()->center().y() + 3),
                              QPoint(mTo->tightRect()->left(), mTo->tightRect()->center().y()),
                              QPoint(mTo->tightRect()->left() - 6, mTo->tightRect()->center().y() - 3)};
            painter->drawPolygon(points, 3);
        } else {
            QPoint points[] = {QPoint(mTo->tightRect()->right() + 6, mTo->tightRect()->center().y() + 3),
                              QPoint(mTo->tightRect()->right(), mTo->tightRect()->center().y()),
                              QPoint(mTo->tightRect()->right() + 6, mTo->tightRect()->center().y() - 3)};
            painter->drawPolygon(points, 3);
        }
    } else {
        if (fromIsHigherThanTo){
            QPoint points[] = {QPoint(mTo->tightRect()->center().x() + 3, mTo->tightRect()->top() - 6),
                              QPoint(mTo->tightRect()->center().x(), mTo->tightRect()->top()),
                              QPoint(mTo->tightRect()->center().x() - 3, mTo->tightRect()->top() - 6)};
            painter->drawPolygon(points, 3);
        } else {
            QPoint points[] = {QPoint(mTo->tightRect()->center().x() + 3, mTo->tightRect()->bottom() + 6),
                              QPoint(mTo->tightRect()->center().x(), mTo->tightRect()->bottom()),
                              QPoint(mTo->tightRect()->center().x() - 3, mTo->tightRect()->bottom() + 6)};
            painter->drawPolygon(points, 3);
        }
    }
}
