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
#include <QTextStream>

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

    NetGraph* mNetGraph;

    AddNode* addNodeDialog;
    bool addNodeDialogOpened;

    ArrowButton* arrowButton;

    QPoint* nodePosition;

    bool mModified;

    void operateAddNodeDialog(QMouseEvent* event, bool editMode = false, Node *editable = nullptr);
    void operateDeleteNodeDialog(Node* selected);
    void operateDeleteArrowDialog(Arrow* selected);

public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();
    bool isModified();
    void save(QTextStream& stream);
    void load(QTextStream& stream);

    void mousePress(QMouseEvent* event);
    void mouseMove(QMouseEvent * event);
    void paint(QPainter* painter);

    void addNode();
    void editNode();

signals:
    void modification();

public slots:
    void modified();
    void saved();
    void addOrEditNode();
    void checkArrowAddRequest(Node* from, Node* to);
};

#endif // EDITOR_H
