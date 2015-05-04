#include "arrow.h"

Arrow::Arrow() :
    mPrimitives(new QVector<Primitive*>()), leadsToSubnet(false), mFrom(nullptr), mTo (nullptr),
    mExpression(nullptr), rects(new QVector<QRect>), drawPath(nullptr), drawHead(nullptr), labelPosition(nullptr)
{
    calculatePathsAndRect();
}

Arrow::Arrow(Node *from, Node *to, QString *expression) :
mPrimitives(new QVector<Primitive*>), leadsToSubnet(false), mFrom(from), mTo(to),
  mExpression(expression), rects(new QVector<QRect>),drawPath(nullptr), drawHead(nullptr), labelPosition(nullptr)
{
    mFrom->addArrowOut(this);
    mTo->addArrowIn(this);
    calculatePathsAndRect();
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
    if (leadsToSubnet){
        pen.setStyle(Qt::DashLine);
    }
    painter->setPen(pen);

    painter->drawPolyline(drawPath, drawPathLength);

    painter->setBrush(brush);

    painter->drawPolygon(drawHead, 3);

    if (mFrom->type() == Node::NodeType::CaseNode || mFrom->type() == Node::NodeType::ProximityNode)
        painter->drawText(*labelPosition, *mExpression);
}

bool Arrow::contains(const QPoint &point)
{
    for (const auto& rect : *rects){
        if (rect.contains(point)){
            return true;
        }
    }
    return false;
}

const Node *Arrow::from()
{
    return mFrom;
}

const Node *Arrow::to()
{
    return mTo;
}

void Arrow::calculatePathsAndRect()
{
    if (!rects->isEmpty())
        rects->clear();
    if (drawPath)
        delete drawPath;
    if (drawHead)
        delete drawHead;
    if (labelPosition)
        delete labelPosition;

    bool fromIsLeftFromTo;
    bool fromIsHigherThanTo;
    bool targetIsSide;

    fromIsLeftFromTo = (mFrom->tightRect()->x() < mTo->tightRect()->x());

    fromIsHigherThanTo = (mFrom->tightRect()->center().y() < mTo->tightRect()->center().y());

    int xDistance = abs(mFrom->tightRect()->center().x() - mTo->tightRect()->center().x());
    int yDistance = abs(mFrom->tightRect()->center().y() - mTo->tightRect()->center().y());

    //arrow body

    if (abs(xDistance - yDistance) < 50){
        //side of from, top/bottom of to
        targetIsSide = false;

        drawPath = new QPoint[3];
        drawPathLength = 3;

        drawPath[0] = QPoint((fromIsLeftFromTo)? mFrom->tightRect()->right() : mFrom->tightRect()->left(),
                              mFrom->tightRect()->center().y());
        drawPath[1] = QPoint(mTo->tightRect()->center().x(),
                             mFrom->tightRect()->center().y());
        drawPath[2] = QPoint(mTo->tightRect()->center().x(),
                             (mFrom->tightRect()->center().y() < mTo->tightRect()->center().y())?
                                 mTo->tightRect()->top() : mTo->tightRect()->bottom());

        //rects
        rects->append(QRect((fromIsLeftFromTo)? drawPath[0].x() : drawPath[1].x(),
                      drawPath[0].y() - 3,
                abs(drawPath[0].x() - drawPath[1].x()),
                          6));
        rects->append(QRect(drawPath[1].x() - 3,
                      (fromIsHigherThanTo)? drawPath[1].y() : drawPath[2].y(),
                6,
                          abs(drawPath[1].y() - drawPath[2].y())));

    } else if (xDistance < yDistance){
        //top to bottom
        targetIsSide = false;
        if (fromIsHigherThanTo){
            drawPath = new QPoint[4];
            drawPathLength = 4;
            drawPath[0] = QPoint(mFrom->tightRect()->center().x(),
                                 mFrom->tightRect()->bottom());
            drawPath[1] = QPoint(mFrom->tightRect()->center().x(),
                                 mTo->tightRect()->top() + (mFrom->tightRect()->bottom() - mTo->tightRect()->top())/2);

            drawPath[2] = QPoint(mTo->tightRect()->center().x(),
                                 mTo->tightRect()->top() + (mFrom->tightRect()->bottom() - mTo->tightRect()->top())/2);
            drawPath[3] = QPoint(mTo->tightRect()->center().x(),
                                 mTo->tightRect()->top());
            //rects
            rects->append(QRect(drawPath[0].x() - 3,
                    drawPath[0].y(),
                    6,
                    abs(drawPath[1].y() - drawPath[0].y())));
            rects->append(QRect((fromIsLeftFromTo)? drawPath[1].x() : drawPath[2].x(),
                          drawPath[1].y() - 3,
                    abs(drawPath[2].x() - drawPath[1].x()),
                              5));
            rects->append(QRect(drawPath[2].x() - 3,
                    drawPath[2].y(),
                    6,
                    abs(drawPath[3].y() - drawPath[2].y())));

        } else {
            drawPath = new QPoint[4];
            drawPathLength = 4;
            drawPath[0] = QPoint(mFrom->tightRect()->center().x(),
                                mFrom->tightRect()->top());
            drawPath[1] = QPoint(mFrom->tightRect()->center().x(),
                                 mTo->tightRect()->bottom() + (mFrom->tightRect()->top() - mTo->tightRect()->bottom())/2);
            drawPath[2] = QPoint(mTo->tightRect()->center().x(),
                                 mTo->tightRect()->bottom() + (mFrom->tightRect()->top() - mTo->tightRect()->bottom())/2);
            drawPath[3] = QPoint(mTo->tightRect()->center().x(),
                                 mTo->tightRect()->bottom());
            //rects
            rects->append(QRect(drawPath[1].x() - 3,
                    drawPath[1].y(),
                    6,
                    abs(drawPath[0].y() - drawPath[1].y())));
            rects->append(QRect((fromIsLeftFromTo)? drawPath[1].x() : drawPath[2].x(),
                          drawPath[1].y() - 3,
                    abs(drawPath[2].x() - drawPath[1].x()),
                              6));
            rects->append(QRect(drawPath[3].x() - 3,
                    drawPath[3].y(),
                    6,
                    abs(drawPath[2].y() - drawPath[3].y())));
        }
    } else {
        //side of from, side of to
        targetIsSide = true;
        drawPath = new QPoint[4];
        drawPathLength = 4;
        drawPath[0] = QPoint((fromIsLeftFromTo)? mFrom->tightRect()->right() : mFrom->tightRect()->left(),
                             mFrom->tightRect()->center().y());
        drawPath[1] = QPoint(mFrom->tightRect()->right() + (mTo->tightRect()->left() - mFrom->tightRect()->right())/2,
                             mFrom->tightRect()->center().y());
        drawPath[2] = QPoint(mFrom->tightRect()->right() + (mTo->tightRect()->left() - mFrom->tightRect()->right())/2,
                             mTo->tightRect()->center().y());
        drawPath[3] = QPoint((fromIsLeftFromTo)? mTo->tightRect()->left() : mTo->tightRect()->right(),
                             mTo->tightRect()->center().y());
        //rects
        rects->append(QRect((fromIsLeftFromTo)? drawPath[0].x() : drawPath[1].x(),
                      drawPath[0].y() - 2,
                abs(drawPath[0].x() - drawPath[1].x()),
                          4));
        rects->append(QRect(drawPath[1].x() - 2,
                      (fromIsHigherThanTo)? drawPath[1].y() : drawPath[2].y(),
                4,
                          abs(drawPath[2].y() - drawPath[1].y())));
        rects->append(QRect((fromIsLeftFromTo)? drawPath[2].x() : drawPath[3].x(),
                      drawPath[2].y() - 2,
                abs(drawPath[2].x() - drawPath[3].x()),
                          4));
    }

    //draw arrow head

    drawHead = new QPoint[3];

    if (targetIsSide){
        if (fromIsLeftFromTo){
            drawHead[0] = QPoint(mTo->tightRect()->left() - 6, mTo->tightRect()->center().y() + 3);
            drawHead[1] = QPoint(mTo->tightRect()->left(), mTo->tightRect()->center().y());
            drawHead[2] = QPoint(mTo->tightRect()->left() - 6, mTo->tightRect()->center().y() - 3);
        } else {
            drawHead[0] = QPoint(mTo->tightRect()->right() + 6, mTo->tightRect()->center().y() + 3);
            drawHead[1] = QPoint(mTo->tightRect()->right(), mTo->tightRect()->center().y());
            drawHead[2] = QPoint(mTo->tightRect()->right() + 6, mTo->tightRect()->center().y() - 3);
        }
    } else {
        if (fromIsHigherThanTo){
            drawHead[0] = QPoint(mTo->tightRect()->center().x() + 3, mTo->tightRect()->top() - 6);
            drawHead[1] = QPoint(mTo->tightRect()->center().x(), mTo->tightRect()->top());
            drawHead[2] = QPoint(mTo->tightRect()->center().x() - 3, mTo->tightRect()->top() - 6);
        } else {
            drawHead[0] = QPoint(mTo->tightRect()->center().x() + 3, mTo->tightRect()->bottom() + 6);
            drawHead[1] = QPoint(mTo->tightRect()->center().x(), mTo->tightRect()->bottom());
            drawHead[2] = QPoint(mTo->tightRect()->center().x() - 3, mTo->tightRect()->bottom() + 6);
        }
    }

    //draw expression/constant
    if (mFrom->type() == Node::NodeType::CaseNode || mFrom->type() == Node::NodeType::ProximityNode){
        if (targetIsSide){
            if (fromIsLeftFromTo){
                labelPosition = new QPoint(mFrom->tightRect()->right() + 5,
                                           mFrom->tightRect()->center().y() - 3);
            } else {
                labelPosition = new QPoint(mFrom->tightRect()->left() - 5,
                                           mFrom->tightRect()->center().y() - 3);
            }
        } else {
            if (fromIsHigherThanTo){
                labelPosition = new QPoint(mFrom->tightRect()->center().x() + 3,
                                           mFrom->tightRect()->bottom() - 5);
            } else {
                labelPosition = new QPoint(mFrom->tightRect()->center().x() + 3,
                                           mFrom->tightRect()->top() + 5);
            }
        }
    }

}
