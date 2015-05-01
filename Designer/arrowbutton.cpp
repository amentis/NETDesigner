#include "arrowbutton.h"

ArrowButton::ArrowButton(QObject *parent) : QObject(parent),
    mRect(nullptr), mSelected(nullptr), mTarget(nullptr),
    mVisible(false), mHovered(false), mActive(false), mHasTarget(false)
{
    connect(this, &ArrowButton::visibleChanged, this, &ArrowButton::updated);
    connect(this, &ArrowButton::hoveredChanged, this, &ArrowButton::updated);
    connect(this, &ArrowButton::activeChanged, this, &ArrowButton::updated);
    connect(this, &ArrowButton::hasTargetChanged, this, &ArrowButton::updated);
}

ArrowButton::~ArrowButton()
{
    delete mRect;
    mSelected = nullptr;
}

QRect *ArrowButton::rect()
{
    return mRect;
}

Node *ArrowButton::getSelected()
{
    return mSelected;
}

void ArrowButton::paint(QPainter *painter)
{

    QBrush black(Qt::black);
    QBrush grey = (Qt::lightGray);
    QPen thinPen(black, 1);
    QPen thickPen(black, 2);

    if (mVisible || mActive){
        if (mHovered || mActive){
            painter->setPen(thinPen);
            painter->setBrush(grey);
            painter->drawRect(*rect());
        }
        painter->setPen(thickPen);
        painter->drawLine(mRect->bottomLeft(), mRect->topRight());
        painter->setBrush(black);
        QPoint points[] = {QPoint(mRect->topRight().x() - 5, mRect->topRight().y()),
                           QPoint(mRect->topRight()),
                           QPoint(mRect->topRight().x(), mRect->topRight().y() + 5)};
        painter->drawPolygon(points, 3);
    }

    if (mActive && mHasTarget){
        painter->setPen(thinPen);
        painter->setBrush(grey);
        painter->drawRect(*mTargetRect);

        painter->setPen(thickPen);
        painter->drawLine(mTargetRect->bottomLeft(), mTargetRect->topRight());
        painter->setBrush(black);
        QPoint targetPoints[] = {QPoint(mTargetRect->topRight().x() - 5, mTargetRect->topRight().y()),
                                 QPoint(mTargetRect->topRight()),
                                 QPoint(mTargetRect->topRight().x(), mTargetRect->topRight().y() + 5)};
        painter->drawPolygon(targetPoints, 3);
    }

}

void ArrowButton::setVisible(bool visible, Node *selected)
{
    if (visible == mVisible && selected == mSelected)
        return;
    if (visible){
        mSelected = selected;
        QRect* nodeRect;
        nodeRect = selected->rect();
        if (!nodeRect)
            return;
        delete mRect;
        mRect = new QRect(nodeRect->right() - 12, nodeRect->top() + 5, 15, 15);
    }
    if (!(!mVisible && !visible)){
        mVisible = visible;
        emit visibleChanged();
    }

}

void ArrowButton::setHovered(bool hovered)
{
    if (hovered != mHovered){
        mHovered = hovered;
        emit hoveredChanged();
    }
}

void ArrowButton::setActive(bool active)
{
    if (active != mActive){
        mActive = active;
        emit activeChanged();
    }
    if (!active){
        if (mSelected && mTarget){
            emit arrowAddRequest(mSelected, mTarget);
            setHovered(false);
        }
    }
}

void ArrowButton::setTarget(bool hasTarget,Node *target)
{
    if ((mHasTarget == hasTarget && mTarget == target) || (target == mSelected && mSelected))
        return;
    if (hasTarget){
        mTarget = target;
        QRect* nodeRect;
        nodeRect = target->rect();
        if (!nodeRect)
            return;
        mTargetRect = new QRect(nodeRect->right() - 12, nodeRect->top() + 5, 15, 15);
    }
    mHasTarget = hasTarget;
    emit hasTargetChanged();
}

bool ArrowButton::active()
{
    return mActive;
}

