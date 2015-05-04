#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QBoxLayout>
#include <QMessageBox>

class Canvas;
#include "NetGraph/node.h"
#include "NetGraph/arrow.h"
#include "NetGraph/netgraph.h"

#include "canvas.h"
#include "addnode.h"
#include "arrowbutton.h"
#include "addarrowexpressiondialog.h"


class Editor : public QWidget
{
    Q_OBJECT

private:
    Canvas* canvas;

    NetGraph* netGraph;

    AddNode* addNodeDialog;
    bool addNodeDialogOpened;

    ArrowButton* arrowButton;

    QPoint* nodePosition;

    bool modified;

    void operateAddNodeDialog(QMouseEvent* event, bool editMode = false, Node *editable = nullptr);
    void operateDeleteNodeDialog(Node* selected);
    void operateDeleteArrowDialog(Arrow* selected);

public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();
    bool isModified();
    bool save();

    void mousePress(QMouseEvent* event);
    void mouseMove(QMouseEvent * event);
    void paint(QPainter* painter);

    void addNode();
    void editNode();

signals:

public slots:
    void addOrEditNode();
    void checkArrowAddRequest(Node* from, Node* to);
};

#endif // EDITOR_H
