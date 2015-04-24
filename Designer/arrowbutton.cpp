#include "arrowbutton.h"

ArrowButton::ArrowButton(QObject *parent) : QObject(parent), mRect(nullptr), mSelected(nullptr), mVisible(false) {}

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
    if (mVisible){
        if (mHovered){
            QBrush grey = (Qt::lightGray);

            painter->setBrush(grey);
            painter->drawRect(*rect());
        }

        QBrush black(Qt::black);
        QPen foreground(black, 2);
        painter->setPen(foreground);
        painter->drawLine(mRect->bottomLeft(), mRect->topRight());
        painter->setBrush(black);
        QPoint points[] = {QPoint(mRect->topRight().x() - 5, mRect->topRight().y()),
                           QPoint(mRect->topRight()),
                           QPoint(mRect->topRight().x(), mRect->topRight().y() + 5)};
        painter->drawPolygon(points, 3);
    }
}

void ArrowButton::setVisible(bool visible, Node *selected)
{
    if (visible == mVisible && selected == mSelected)
        return;
    if (visible){
        QRect* nodeRect;
        switch (selected->type()) {
        case Node::NodeType::StartNode: nodeRect = (dynamic_cast<StartNode*>(selected))->rect(); break;
        case Node::NodeType::EndNode: nodeRect = (dynamic_cast<EndNode*>(selected))->rect(); break;
        case Node::NodeType::OrdinaryNode: nodeRect = (dynamic_cast<OrdinaryNode*>(selected))->rect(); break;
        case Node::NodeType::CaseNode: nodeRect = (dynamic_cast<CaseNode*>(selected))->rect(); break;
        case Node::NodeType::ProximityNode: nodeRect = (dynamic_cast<ProximityNode*>(selected))->rect(); break;
        }
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

