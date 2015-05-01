#include "editor.h"

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
        netGraph->removeNode(selected);
    }
}

Editor::Editor(QWidget *parent) : QWidget(parent),
    canvas(new Canvas(this,this)), netGraph(new NetGraph(this)), addNodeDialog(new AddNode(this)),
    addNodeDialogOpened(false), arrowButton(new ArrowButton()), nodePosition(nullptr),
    modified(false)
{
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    layout->addWidget(canvas);
    setLayout(layout);

    connect(netGraph, &NetGraph::contentModified, canvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));
    connect(addNodeDialog, &AddNode::accepted, this, &Editor::addOrEditNode);
    connect(arrowButton, &ArrowButton::updated, canvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));
    connect(arrowButton, &ArrowButton::arrowAddRequest, this, &Editor::checkArrowAddRequest);
}

Editor::~Editor()
{
    delete arrowButton;
}

bool Editor::isModified()
{
    return modified;
}

bool Editor::save()
{
    //TODO: implement save, is ok = true
    return true;
}

void Editor::addNode()
{
    Node* node;
    addNodeDialog->getResult(node);
    netGraph->addNode(node, nodePosition);
}

void Editor::editNode()
{
    Node* node;
    Node::NodeType type;
    QString* expression;
    addNodeDialog->getResult(node, type, expression);
    netGraph->editNode(node, type, expression);
}

void Editor::addOrEditNode()
{
    if (addNodeDialog->isEditMode())
        editNode();
    else
        addNode();
}

void Editor::checkArrowAddRequest(Node *from, Node *to)
{
    if (from->type() == Node::NodeType::CaseNode){
        AddArrowExpressionDialog dialog(true, this);
        if (dialog.exec() == QDialog::Accepted){
            netGraph->addArrow(from, to, new QString(dialog.expression()));
        }
    } else if (from->type() == Node::NodeType::ProximityNode){
        AddArrowExpressionDialog dialog(false, this);
        if (dialog.exec() == QDialog::Accepted){
            netGraph->addArrow(from, to, new QString(dialog.expression()));
        }
    } else {
        netGraph->addArrow(from, to);
    }
}



void Editor::paint(QPainter *painter)
{
    for (const auto& node : *netGraph->getNodes()){
        node->paint(painter);
    }

    for (const auto& arrow : *netGraph->getArrows()){
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

    for (const auto& node : *netGraph->getNodes()) {
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
    for (const auto& node : *netGraph->getNodes()) {
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
