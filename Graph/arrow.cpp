#include "arrow.h"

#include <qmath.h>

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
mPrimitives(new QVector<Primitive*>), mPrimitiveArguments(new QHash<QString, QString*>), leadsToSubnet(false),
  mFrom(from), mTo(to), mExpression(expression), mRects(new QVector<QRect>),drawPath(nullptr), drawHead(nullptr), labelPosition(nullptr),
callTo(nullptr), mLoops(0), mRecursion(0), mExecuteBack(true)
{
    mFrom->addArrowOut(this);
    mTo->addArrowIn(this);
    calculatePathsAndRect();
}

Arrow::~Arrow()
{
    delete mPrimitives;
    delete mPrimitiveArguments;
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
    return mPrimitiveArguments->value(*primitiveName);
}

void Arrow::setArgumentsForPrimitive(QString* primitiveName, QString* arguments)
{
    mPrimitiveArguments->insert(*primitiveName, arguments);
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

    int xFrom;
    int yFrom;
    int xTo;
    int yTo;

    //arrow body


    if (abs(xDistance - yDistance) < 50){
        //side of from, top/bottom of to
        targetIsSide = false;

        //calculate arrow position relative to from

        QVector<Arrow*> arrowsDrawnAtFrom(*(mFrom->arrowsOut()));
        for (const auto& arrow : *(mTo->arrowsLeadingToOtherNode(mFrom))){
            arrowsDrawnAtFrom.append(arrow);
        }

        // divide the side of the rect into even parts
        unsigned segmentSize = mFrom->tightRect()->height() / (arrowsDrawnAtFrom.size() + 2);

        // determine index of the arrow relative to from

        unsigned idx = arrowsDrawnAtFrom.indexOf(this);

        unsigned index;

        if ((arrowsDrawnAtFrom.size()%2) && idx == 0)
            index = qFloor(((float)(arrowsDrawnAtFrom.size())) / 2);
        else
            index = (!(idx%2))? idx/2 :
                                  ((arrowsDrawnAtFrom.size()%2)? qFloor(((float)(arrowsDrawnAtFrom.size())) / 2) :
                                                                 arrowsDrawnAtFrom.size() / 2) +
                                (arrowsDrawnAtFrom.size() - qCeil(idx/2));

        // determine offset from edge of rect

        unsigned offset = (index == 0)? (qAbs(mFrom->tightRect()->center().y() - mFrom->tightRect()->bottom())) :
                                        (index * segmentSize);

        yFrom = mFrom->tightRect()->bottom() - offset; // y coordinate to draw from

        //calculate arrows position relatice to to

        QVector<Arrow*> arrowsDrawnAtTo(*(mTo->arrowsIn()));
        for (const auto& arrow : *(mTo->arrowsLeadingToOtherNode(mFrom))){
            arrowsDrawnAtTo.append(arrow);
        }

        // divide the side of the rect into even parts
        segmentSize = mTo->tightRect()->width() / (arrowsDrawnAtTo.size() + 2);

        // determine index of the arrow relative to to

        idx = arrowsDrawnAtTo.indexOf(this);

        if ((arrowsDrawnAtTo.size()%2) && idx == 0)
            index = qFloor(((float)arrowsDrawnAtTo.size()) / 2);
        else
            index = (!(idx%2))? idx/2 :
                                  ((arrowsDrawnAtTo.size()%2)? qFloor(((float)(arrowsDrawnAtTo.size())) / 2) :
                                                               arrowsDrawnAtTo.size() / 2) +
                                  (arrowsDrawnAtTo.size() - qCeil(idx/2));

        // determine offset from edge of rect

        offset = (index == 0)? (qAbs(mTo->tightRect()->center().x() - mTo->tightRect()->left())) : (index * segmentSize);

        xTo = mTo->tightRect()->left() + offset; // x coordinate to draw from

        drawPath = new QPoint[3];
        drawPathLength = 3;

        xFrom = (fromIsLeftFromTo)? mFrom->tightRect()->right() : mFrom->tightRect()->left();
        yTo = (yFrom < mTo->tightRect()->center().y())?
                    mTo->tightRect()->top() : mTo->tightRect()->bottom();

        drawPath[0] = QPoint(xFrom, yFrom);
        drawPath[1] = QPoint(xTo, yFrom);
        drawPath[2] = QPoint(xTo, yTo);

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

        //calculate arrow position relative to from

        QVector<Arrow*> arrowsDrawnAtFrom(*(mFrom->arrowsOut()));
        for (const auto& arrow : *(mTo->arrowsLeadingToOtherNode(mFrom))){
            arrowsDrawnAtFrom.append(arrow);
        }

        // divide the side of the rect into even parts
        unsigned segmentSize = mFrom->tightRect()->width() / (arrowsDrawnAtFrom.size() + 2);

        // determine index of the arrow relative to from

        unsigned idx = arrowsDrawnAtFrom.indexOf(this);

        unsigned index;

        if ((arrowsDrawnAtFrom.size()%2) && idx == 0)
            index = qFloor(((float)(arrowsDrawnAtFrom.size())) / 2);
        else
            index = (!(idx%2))? idx/2 :
                                  ((arrowsDrawnAtFrom.size()%2)? qFloor(((float)(arrowsDrawnAtFrom.size())) / 2) :
                                                                 arrowsDrawnAtFrom.size() / 2) +
                                (arrowsDrawnAtFrom.size() - qCeil(idx/2));

        // determine offset from edge of rect

        unsigned offset = (index == 0)? (qAbs(mFrom->tightRect()->center().x() - mFrom->tightRect()->left())) :
                                        (index * segmentSize);

        xFrom = mFrom->tightRect()->left() + offset; // x coordinate to draw from

        //calculate arrows position relative to to

        QVector<Arrow*> arrowsDrawnAtTo(*(mTo->arrowsIn()));
        for (const auto& arrow : *(mTo->arrowsLeadingToOtherNode(mFrom))){
            arrowsDrawnAtTo.append(arrow);
        }

        // divide the side of the rect into even parts
        segmentSize = mTo->tightRect()->width() / (arrowsDrawnAtTo.size() + 2);

        // determine index of the arrow relative to to

        idx = arrowsDrawnAtTo.indexOf(this);

        if ((arrowsDrawnAtTo.size()%2) && idx == 0)
            index = qFloor(((float)arrowsDrawnAtTo.size()) / 2);
        else
            index = (!(idx%2))? idx/2 :
                                  ((arrowsDrawnAtTo.size()%2)? qFloor(((float)(arrowsDrawnAtTo.size())) / 2) :
                                                               arrowsDrawnAtTo.size() / 2) +
                                  (arrowsDrawnAtTo.size() - qCeil(idx/2));

        // determine offset from edge of rect

        offset = (index == 0)? (qAbs(mTo->tightRect()->center().x() - mTo->tightRect()->left())) : (index * segmentSize);

        xTo = mTo->tightRect()->left() + offset; // x coordinate to draw from

        if (fromIsHigherThanTo){
            drawPath = new QPoint[4];
            drawPathLength = 4;
            yFrom = mFrom->tightRect()->bottom();
            yTo = mTo->tightRect()->top();
            drawPath[0] = QPoint(xFrom, yFrom);
            drawPath[1] = QPoint(xFrom,
                                 mTo->tightRect()->top() - offset/2 + (mFrom->tightRect()->bottom() - mTo->tightRect()->top())/2);

            drawPath[2] = QPoint(xTo,
                                 mTo->tightRect()->top() - offset/2 + (mFrom->tightRect()->bottom() - mTo->tightRect()->top())/2);
            drawPath[3] = QPoint(xTo, yTo);
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
            yFrom = mFrom->tightRect()->top();
            yTo = mTo->tightRect()->bottom();
            drawPath = new QPoint[4];
            drawPathLength = 4;
            drawPath[0] = QPoint(xFrom, yFrom);
            drawPath[1] = QPoint(xFrom,
                                 mTo->tightRect()->bottom() - offset/2 +
                                 (mFrom->tightRect()->top() - mTo->tightRect()->bottom())/2);
            drawPath[2] = QPoint(xTo,
                                 mTo->tightRect()->bottom() - offset/2 +
                                 (mFrom->tightRect()->top() - mTo->tightRect()->bottom())/2);
            drawPath[3] = QPoint(xTo, yTo);
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

        //calculate arrow position relative to from

        QVector<Arrow*> arrowsDrawnAtFrom(*(mFrom->arrowsOut()));
        for (const auto& arrow : *(mTo->arrowsLeadingToOtherNode(mFrom))){
            arrowsDrawnAtFrom.append(arrow);
        }

        // divide the side of the rect into even parts
        unsigned segmentSize = mFrom->tightRect()->height() / (arrowsDrawnAtFrom.size() + 2);

        // determine index of the arrow relative to from

        unsigned idx = arrowsDrawnAtFrom.indexOf(this);

        unsigned index;

        if ((arrowsDrawnAtFrom.size()%2) && idx == 0)
            index = qFloor(((float)(arrowsDrawnAtFrom.size())) / 2);
        else
            index = (!(idx%2))? idx/2 :
                                  ((arrowsDrawnAtFrom.size()%2)? qFloor(((float)(arrowsDrawnAtFrom.size())) / 2) :
                                                                 arrowsDrawnAtFrom.size() / 2) +
                                (arrowsDrawnAtFrom.size() - qCeil(idx/2));

        // determine offset from edge of rect

        unsigned offset = (index == 0)? (qAbs(mFrom->tightRect()->center().y() - mFrom->tightRect()->bottom())) :
                                        (index * segmentSize);

        yFrom = mFrom->tightRect()->bottom() - offset; // x coordinate to draw from

        //calculate arrows position relatice to to

        QVector<Arrow*> arrowsDrawnAtTo(*(mTo->arrowsIn()));
        for (const auto& arrow : *(mTo->arrowsLeadingToOtherNode(mFrom))){
            arrowsDrawnAtTo.append(arrow);
        }

        // divide the side of the rect into even parts
        segmentSize = mTo->tightRect()->height() / (arrowsDrawnAtTo.size() + 2);

        // determine index of the arrow relative to to

        idx = arrowsDrawnAtTo.indexOf(this);

        if ((arrowsDrawnAtTo.size()%2) && idx == 0)
            index = qFloor(((float)arrowsDrawnAtTo.size()) / 2);
        else
            index = (!(idx%2))? idx/2 :
                                  ((arrowsDrawnAtTo.size()%2)? qFloor(((float)(arrowsDrawnAtTo.size())) / 2) :
                                                               arrowsDrawnAtTo.size() / 2) +
                                  (arrowsDrawnAtTo.size() - qCeil(idx/2));

        // determine offset from edge of rect

        offset = (index == 0)? (qAbs(mTo->tightRect()->center().y() - mTo->tightRect()->bottom())) : (index * segmentSize);

        yTo = mTo->tightRect()->bottom() - offset; // x coordinate to draw from


        drawPath = new QPoint[4];
        drawPathLength = 4;
        xFrom = (fromIsLeftFromTo)? mFrom->tightRect()->right() : mFrom->tightRect()->left();
        xTo = (fromIsLeftFromTo)? mTo->tightRect()->left() : mTo->tightRect()->right();
        drawPath[0] = QPoint(xFrom, yFrom);
        drawPath[1] = QPoint(mFrom->tightRect()->right() - offset/2 + (mTo->tightRect()->left() - mFrom->tightRect()->right())/2,
                             yFrom);
        drawPath[2] = QPoint(mFrom->tightRect()->right() - offset/2 + (mTo->tightRect()->left() - mFrom->tightRect()->right())/2,
                             yTo);
        drawPath[3] = QPoint(xTo, yTo);
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
            drawHead[0] = QPoint(xTo - 6, yTo + 3);
            drawHead[1] = QPoint(xTo, yTo);
            drawHead[2] = QPoint(xTo - 6, yTo - 3);
        } else {
            drawHead[0] = QPoint(xTo + 6, yTo + 3);
            drawHead[1] = QPoint(xTo, yTo);
            drawHead[2] = QPoint(xTo + 6, yTo - 3);
        }
    } else {
        if (fromIsHigherThanTo){
            drawHead[0] = QPoint(xTo + 3, yTo - 6);
            drawHead[1] = QPoint(xTo, yTo);
            drawHead[2] = QPoint(xTo - 3, yTo - 6);
        } else {
            drawHead[0] = QPoint(xTo + 3, yTo + 6);
            drawHead[1] = QPoint(xTo, yTo);
            drawHead[2] = QPoint(xTo - 3, yTo + 6);
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
