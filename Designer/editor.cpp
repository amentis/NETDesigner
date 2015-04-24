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

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    canvas = new Canvas(this, this);
    layout->addWidget(canvas);
    setLayout(layout);
    modified = false;

    netGraph = new NetGraph(this);

    connect(netGraph, &NetGraph::contentModified, canvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));

    addNodeDialog = new AddNode(this);
    connect(addNodeDialog, &AddNode::accepted, this, &Editor::addOrEditNode);
    addNodeDialogOpened = false;

    arrowButton = new ArrowButton();

    connect(arrowButton, &ArrowButton::visibleChanged, canvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));
    connect(arrowButton, &ArrowButton::hoveredChanged, canvas, static_cast<void (Canvas::*)()>(&Canvas::repaint));

    nodePosition = nullptr;

}

Editor::~Editor()
{
    delete canvas;
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



void Editor::paint(QPainter *painter)
{
    for (const auto& node : *netGraph->getNodes()){
        node->paint(painter);
    }
    arrowButton->paint(painter);
}

void Editor::mousePress(QMouseEvent *event)
{
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
        arrowButton->setVisible(true, selected);
        if (arrowButton->rect()->contains(QWidget::mapToParent(event->pos()))){
            arrowButton->setHovered(true);
        } else {
            arrowButton->setHovered(false);
        }
    } else {
        arrowButton->setVisible(false, nullptr);
    }
}
