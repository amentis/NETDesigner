#include "editor.h"

#include <QtGui>
#include <QtWidgets>

#include "NetGraph/node.h"
#include "NetGraph/arrow.h"
#include "NetGraph/netgraph.h"

#include "addnode.h"
#include "arrowbutton.h"
#include "addarrowexpressiondialog.h"

#include "canvas.h"

void Editor::operateAddNodeDialog(QMouseEvent *event, bool editMode, Node* editable)
{
    if (!addNodeDialogOpened){
        addNodeDialog->move(QWidget::mapToGlobal(event->pos()));
        nodePosition = new QPoint(QWidget::mapToParent(event->pos()));
        if (editMode)
            addNodeDialog->showEditMode(editable);
        else
            addNodeDialog->showAddMode();
        addNodeDialogOpened = true;
    } else {
        addNodeDialog->hide();
        addNodeDialogOpened = false;
    }
}

void Editor::operateDeleteNodeDialog(Node *selected)
{
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, tr("NETDesigner"), tr("Are you sure you want to delete this node?"),
                      QMessageBox::Yes | QMessageBox::No);
    if (confirm == QMessageBox::Yes){
        mNetGraph->removeNode(selected);
    }
}

void Editor::operateDeleteArrowDialog(Arrow *selected)
{
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, tr("NetDesigner"), tr("Are you sure you want to delete this arrow?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (confirm == QMessageBox::Yes){
        mNetGraph->removeArrow(selected);
    }
}

Editor::Editor(QWidget *parent) : QWidget(parent),
    mCanvas(new Canvas(this,this)), mNetGraph(new NetGraph(this)), addNodeDialog(new AddNode(this)),
    addNodeDialogOpened(false), arrowButton(new ArrowButton(this)), nodePosition(nullptr),
    mModified(false)
{
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    layout->addWidget(mCanvas);
    setLayout(layout);

    connect(mNetGraph, &NetGraph::contentModified, this, &Editor::modification);
    connect(this, &Editor::modification, this, &Editor::modified);
    connect(this, &Editor::modification, mCanvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));
    connect(addNodeDialog, &AddNode::accepted, this, &Editor::addOrEditNode);
    connect(arrowButton, &ArrowButton::updated, mCanvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));
    connect(arrowButton, &ArrowButton::arrowAddRequest, this, &Editor::checkArrowAddRequest);
}

Editor::~Editor()
{
    delete nodePosition;
}

bool Editor::isModified()
{
    return mModified;
}

void Editor::save(QTextStream &stream)
{
    mNetGraph->saveToStream(stream);
    saved();
}

void Editor::load(QTextStream &stream)
{
    mNetGraph->loadFromStream(stream);
    saved();
}

void Editor::addNode()
{
    Node* node;
    addNodeDialog->getResult(node);
    mNetGraph->addNode(node, nodePosition);
}

void Editor::editNode()
{
    Node* node;
    Node::NodeType type;
    QString* expression;
    addNodeDialog->getResult(node, type, expression);
    mNetGraph->editNode(node, type, expression);
}

void Editor::modified()
{
    mModified = true;
}

void Editor::saved()
{
    mModified = false;
}

void Editor::addOrEditNode()
{
    if (addNodeDialog->editMode())
        editNode();
    else
        addNode();
}

void Editor::checkArrowAddRequest(Node *from, Node *to)
{
    if (from->type() == Node::NodeType::CaseNode){
        AddArrowExpressionDialog dialog(true, this);
        if (dialog.exec() == QDialog::Accepted){
            mNetGraph->addArrow(from, to, dialog.expression());
        }
    } else if (from->type() == Node::NodeType::ProximityNode){
        AddArrowExpressionDialog dialog(false, this);
        if (dialog.exec() == QDialog::Accepted){
            mNetGraph->addArrow(from, to, dialog.expression());
        }
    } else {
        mNetGraph->addArrow(from, to);
    }
}



void Editor::paint(QPainter *painter)
{
    for (const auto& node : *mNetGraph->nodes()){
        node->paint(painter);
    }

    for (const auto& arrow : *mNetGraph->arrows()){
        arrow->paint(painter);
    }

    arrowButton->paint(painter);

}

void Editor::mousePress(QMouseEvent *event)
{

    if (!arrowButton->active()){
        if (arrowButton->rect())
            if (arrowButton->rect()->contains(QWidget::mapToParent(event->pos()))){
                arrowButton->setActive(true);
                return;
            }
    } else {
        arrowButton->setActive(false);
        return;
    }

    for (const auto& arrow : *mNetGraph->arrows()){
        if (arrow->contains(QWidget::mapToParent(event->pos()))){
            if (event->button() == Qt::LeftButton){
                //TODO: edit arrow
            } else if (event->button() == Qt::RightButton){
                operateDeleteArrowDialog(arrow);
            }
            return;
        }
    }

    for (const auto& node : *mNetGraph->nodes()) {
        if (node->rect()->contains(QWidget::mapToParent(event->pos()))){
            if (event->button() == Qt::LeftButton){
                operateAddNodeDialog(event, true, node);
            } else if (event->button() == Qt::RightButton){
                operateDeleteNodeDialog(node);
            }
            return;
        }
    }
    if (event->button() == Qt::LeftButton){
        operateAddNodeDialog(event);
        return;
    }
}

void Editor::mouseMove(QMouseEvent *event)
{
    Node* selected = nullptr;
    for (const auto& node : *mNetGraph->nodes()) {
        if (node->rect()->contains(QWidget::mapToParent(event->pos()))){
            selected = node;
            break;
        }
    }

    if (selected){
        if (arrowButton->active()){
            arrowButton->setTarget(true, selected);
        } else {
            arrowButton->setVisible(true, selected);
            if (arrowButton->rect()->contains(QWidget::mapToParent(event->pos()))){
                arrowButton->setHovered(true);
            } else {
                arrowButton->setHovered(false);
            }
        }
    } else {
        arrowButton->setVisible(false, nullptr);
        if (arrowButton->active()){
            arrowButton->setTarget(false);
        }
    }
}
