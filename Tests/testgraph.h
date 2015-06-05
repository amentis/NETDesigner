#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include <QtTest>

class Graph;
class Arrow;

class TestGraph : public QObject
{
    Q_OBJECT
public:
    TestGraph();

private:
    Graph* graph;
    Arrow* arrow;

private slots:
    void initTestCase();

    //Graph
    void testAddEditRemoveNode();
    void testAddRemoveArrow();
    void testSaveToLoadFromStream();

    //Arrow
    void testOptions();
    void testCallToSubnet();
    void clearPrimitives();
    void testArgumentsAndPrimitives();

};

#endif // TESTGRAPH_H
