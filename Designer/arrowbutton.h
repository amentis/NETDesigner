#ifndef ARROWBUTTON_H
#define ARROWBUTTON_H

#include <QObject>
#include <QRect>
#include <QPoint>
#include <QPainter>
#include <QBrush>
#include <QPen>

#include "../NetGraph/node.h"
#include "../NetGraph/casenode.h"
#include "../NetGraph/endnode.h"
#include "../NetGraph/ordinarynode.h"
#include "../NetGraph/proximitynode.h"
#include "../NetGraph/startnode.h"

class ArrowButton : public QObject
{
    Q_OBJECT
public:
    explicit ArrowButton(QObject *parent = 0);
    ~ArrowButton();

    QRect* rect();
    Node* getSelected();
    void paint(QPainter* painter);

private:
    QRect* mRect;
    Node* mSelected;
    bool mVisible;
    bool mHovered;

signals:
    void visibleChanged();
    void hoveredChanged();

public slots:
    void setVisible(bool visible, Node* selected);
    void setHovered(bool hovered);
};

#endif // ARROWBUTTON_H
