#include "testgraph.h"

#include "graph.h"
#include "arrow.h"
#include "node.h"

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
    //TODO
}

void TestGraph::testAddRemoveArrow()
{
    //TODO
}

void TestGraph::testSaveToLoadFromStream()
{
    //TODO
}

void TestGraph::testOptions()
{
    //TODO
}

void TestGraph::testCallToSubnet()
{
    //TODO
}

void TestGraph::clearPrimitives()
{
    //TODO
}

void TestGraph::testArgumentsAndPrimitives()
{
    //TODO
}

