#include "testgraph.h"

#include <QtTest>

#include "graph.h"
#include "arrow.h"
#include "node.h"

#include "base.h"
#include "primitive.h"

TestGraph::TestGraph(){}

void TestGraph::initTestCase()
{
#ifdef Q_OS_WIN
    QLibrary libGraph("Graph.dll");
#else
    QLibrary libGraph("libGraph");
#endif
    libGraph.load();
    if (!libGraph.isLoaded()){
        qCritical("Cannot load libGraph");
        qApp->exit(qApp->exec());
    }
    graph = new Graph();

}

void TestGraph::testAddEditRemoveNode()
{
    Node node(Node::NodeType::OrdinaryNode, nullptr);
    QPoint* point = new QPoint();
    graph->addNode(&node, point);
    QCOMPARE(graph->nodes()->size(), 1);
    QCOMPARE(graph->nodes()->at(0)->type(), node.type());

    QString newExpression("a+b");

    graph->editNode(&node, Node::NodeType::CaseNode, new QString(newExpression));

    QCOMPARE(graph->nodes()->at(0)->type(), node.type());
    QCOMPARE(node.type(), Node::NodeType::CaseNode);
    QCOMPARE(*(node.expression()), newExpression);

    graph->removeNode(&node);

    QCOMPARE(graph->nodes()->size(), 0);
}

void TestGraph::testAddRemoveArrow()
{

    QPoint* node1Pos = new QPoint(0,0);
    QPoint* node2Pos = new QPoint(1,1);

    Node node1(Node::NodeType::OrdinaryNode, nullptr);
    Node node2(Node::NodeType::CaseNode, "a+b");
    graph->addNode(&node1, node1Pos);
    graph->addNode(&node2, node2Pos);
    graph->addArrow(&node1, &node2, nullptr);
    Arrow* arr = graph->arrows()->at(0);
    graph->addArrow(&node2, &node1, new QString("3"));

    QCOMPARE(graph->arrows()->size(), 2);

    graph->removeArrow(arr);

    QCOMPARE(graph->arrows()->size(), 1);

    graph->removeNode(&node1);

    QCOMPARE(graph->arrows()->size(), 0);

    graph->removeNode(&node2);
}

void TestGraph::testSaveToLoadFromStream()
{
    Node node1;
    Node node2;
    QPoint* node1pos = new QPoint();
    QPoint* node2pos = new QPoint();
    graph->addNode(&node1, node1pos);
    graph->addNode(&node2, node2pos);
    graph->addArrow(&node1, &node2);
    QString saved;
    QTextStream stream(&saved);
    graph->saveToStream(stream);
    Graph graph2;
    QVector<Base*> bases;
    graph2.loadFromStream(stream, &bases);

    QCOMPARE(graph->arrows()->size(), graph2.arrows()->size());
    QCOMPARE(graph->nodes()->size(), graph2.arrows()->size());
}

void TestGraph::testCallToSubnet()
{
    Node node1;
    Node node2;
    QPoint* node1pos = new QPoint();
    QPoint* node2pos = new QPoint();
    graph->addNode(&node1, node1pos);
    graph->addNode(&node2, node2pos);
    graph->addArrow(&node1, &node2, nullptr);
    QString* subnet1 = new QString("subnet");
    graph->arrows()->at(0)->setCall(subnet1);
    QString* subnet2;
    QCOMPARE(graph->arrows()->at(0)->subnetCalled(subnet2), true);
    QCOMPARE(*subnet2, *subnet1);
    graph->arrows()->at(0)->setCall();
    QCOMPARE(graph->arrows()->at(0)->subnetCalled(subnet2), false);
    QVERIFY(!subnet2);
}

void TestGraph::testClearPrimitives()
{
    Primitive primitive1;
    Node node1;
    Node node2;
    QPoint* node1pos = new QPoint();
    QPoint* node2pos = new QPoint();
    graph->addNode(&node1, node1pos);
    graph->addNode(&node2, node2pos);
    graph->addArrow(&node1, &node2, nullptr);

    graph->arrows()->at(0)->addPrimitive(&primitive1);

    graph->arrows()->at(0)->clearPrimitives();

    QCOMPARE(graph->arrows()->at(0)->primitives()->size(), 0);
}

void TestGraph::testArgumentsAndPrimitives()
{
    Base base;
    Primitive primitive1(&base, new QString("primitive"));
    primitive1.addArgument(Primitive::ArgumentType::Int, new QString("a"));

    Node node1;
    Node node2;
    QPoint* node1pos = new QPoint();
    QPoint* node2pos = new QPoint();
    graph->addNode(&node1, node1pos);
    graph->addNode(&node2, node2pos);
    graph->addArrow(&node1, &node2, nullptr);

    graph->arrows()->at(0)->addPrimitive(&primitive1);
    graph->arrows()->at(0)->setArgumentsForPrimitive(primitive1.name(), new QString("1"));

    QCOMPARE(*(graph->arrows()->at(0)->argumentsForPrimitive(primitive1.name())), QString("1"));
}

