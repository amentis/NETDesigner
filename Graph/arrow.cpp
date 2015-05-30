#include "arrow.h"

#include <QtGui>

#include "node.h"

#include "primitive.h"

Arrow::Arrow(QObject *parent) : QObject(parent),
    mPrimitives(new QVector<Primitive*>()), leadsToSubnet(false), mFrom(nullptr), mTo (nullptr),
    mExpression(nullptr), mRects(new QVector<QRect>), drawPath(nullptr), drawHead(nullptr), labelPosition(nullptr),
    callTo(nullptr), mLoops(0), mRecursion(0), mExecuteBack(true)
{
    calculatePathsAndRect();
}

Arrow::Arrow(Node *from, Node *to, QString *expression, QObject *parent) : QObject(parent),
mPrimitives(new QVector<Primitive*>), leadsToSubnet(false), mFrom(from), mTo(to),
  mExpression(expression), mRects(new QVector<QRect>),drawPath(nullptr), drawHead(nullptr), labelPosition(nullptr),
callTo(nullptr), mLoops(0), mRecursion(0), mExecuteBack(true)
{
    mFrom->addArrowOut(this);
    mTo->addArrowIn(this);
    calculatePathsAndRect();
}

Arrow::~Arrow()
{
    delete mPrimitives;
    delete mExpression;
    delete mRects;
    delete drawPath;
    delete drawHead;
    delete labelPosition;
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

    QString primitives;
    for (Primitive* primitive : *mPrimitives){
        primitives.append(primitive->name());
        primitives.append("(");
        primitives.append(argumentsForPrimitive(primitive->name()));
        primitives.append(");");
    }

    if (leadsToSubnet){
        primitives.append("  CALL(");
        primitives.append(callTo);
        primitives.append(")");
    }

    painter->drawText(*primitivesPosition, primitives);

    painter->setBrush(brush);

    painter->drawPolygon(drawHead, 3);

    if (mFrom->type() == Node::NodeType::CaseNode || mFrom->type() == Node::NodeType::ProximityNode)
        painter->drawText(*labelPosition, *mExpression);
}

bool Arrow::contains(const QPoint &point)
{
    for (const auto& rect : *mRects){
        if (rect.contains(point)){
            return true;
        }
    }
    return false;
}

Node *Arrow::from()
{
    return mFrom;
}

Node *Arrow::to()
{
    return mTo;
}
QString *Arrow::expression()
{
    return mExpression;
}

const QVector<Primitive*>*Arrow::primitives()
{
    return mPrimitives;
}

void Arrow::setOptions(int loops, unsigned recursion, bool executeBack)
{
    mLoops = loops;
    mRecursion = recursion;
    mExecuteBack = executeBack;
}

void Arrow::options(int& loops, unsigned& recursion, bool& executeBack)
{
    loops = mLoops;
    recursion = mRecursion;
    executeBack = mExecuteBack;
}

void Arrow::setCall(QString* subNet)
{
    if (subNet){
        callTo = subNet;
        leadsToSubnet = true;
    } else {
        leadsToSubnet = false;
    }
}

void Arrow::clearPrimitives()
{
    mPrimitives->clear();
}

void Arrow::addPrimitive(Primitive* primitive)
{
    mPrimitives->append(primitive);
}

QString*Arrow::argumentsForPrimitive(QString* primitiveName)
{
    return mPrimitiveArguments.value(*primitiveName);
}

void Arrow::setArgumentsForPrimitive(QString* primitiveName, QString* arguments)
{
    mPrimitiveArguments.insert(*primitiveName, arguments);
}

bool Arrow::subnetCalled(QString*& subnetName)
{
    subnetName = callTo;
    return leadsToSubnet;
}


void Arrow::calculatePathsAndRect()
{
    if (!mRects->isEmpty())
        mRects->clear();
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
        mRects->append(QRect((fromIsLeftFromTo)? drawPath[0].x() : drawPath[1].x(),
                      drawPath[0].y() - 3,
                abs(drawPath[0].x() - drawPath[1].x()),
                          6));
        mRects->append(QRect(drawPath[1].x() - 3,
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
            mRects->append(QRect(drawPath[0].x() - 3,
                    drawPath[0].y(),
                    6,
                    abs(drawPath[1].y() - drawPath[0].y())));
            mRects->append(QRect((fromIsLeftFromTo)? drawPath[1].x() : drawPath[2].x(),
                          drawPath[1].y() - 3,
                    abs(drawPath[2].x() - drawPath[1].x()),
                              5));
            mRects->append(QRect(drawPath[2].x() - 3,
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
            mRects->append(QRect(drawPath[1].x() - 3,
                    drawPath[1].y(),
                    6,
                    abs(drawPath[0].y() - drawPath[1].y())));
            mRects->append(QRect((fromIsLeftFromTo)? drawPath[1].x() : drawPath[2].x(),
                          drawPath[1].y() - 3,
                    abs(drawPath[2].x() - drawPath[1].x()),
                              6));
            mRects->append(QRect(drawPath[3].x() - 3,
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
        mRects->append(QRect((fromIsLeftFromTo)? drawPath[0].x() : drawPath[1].x(),
                      drawPath[0].y() - 2,
                abs(drawPath[0].x() - drawPath[1].x()),
                          4));
        mRects->append(QRect(drawPath[1].x() - 2,
                      (fromIsHigherThanTo)? drawPath[1].y() : drawPath[2].y(),
                4,
                          abs(drawPath[2].y() - drawPath[1].y())));
        mRects->append(QRect((fromIsLeftFromTo)? drawPath[2].x() : drawPath[3].x(),
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

    //primitives label
    if (targetIsSide){
            primitivesPosition = new QPoint((fromIsLeftFromTo)?mFrom->rect()->right() + 5 : mTo->rect()->right() + 5,
                                            (mRects->size() == 2)? mRects->at(0).top() - 3: mRects->at(1).top() - 3);
    } else {
        primitivesPosition = new QPoint(mRects->at(1).left() - 4,
                                        mRects->at(1).top());
    }

}
