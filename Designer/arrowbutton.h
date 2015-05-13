#ifndef ARROWBUTTON_H
#define ARROWBUTTON_H

#include <QObject>

class Node;

class QPainter;
class QRect;

class ArrowButton : public QObject
{
    Q_OBJECT
public:
    explicit ArrowButton(QObject *parent = 0);
    ~ArrowButton();

    QRect* rect();
    Node* selected();
    void paint(QPainter* painter);
    bool active();

private:
    QRect* mRect;
    Node* mSelected;
    Node* mTarget;
    QRect* mTargetRect;
    bool mVisible;
    bool mHovered;
    bool mActive;
    bool mHasTarget;

signals:
    void visibleChanged();
    void hoveredChanged();
    void activeChanged();
    void hasTargetChanged();
    void updated();
    void arrowAddRequest(Node* from, Node* to);

public slots:
    void setVisible(bool visible, Node* selected);
    void setHovered(bool hovered);
    void setActive(bool active);
    void setTarget(bool hasTarget, Node* target = nullptr);
};

#endif // ARROWBUTTON_H
