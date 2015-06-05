#include <QtTest>

#include "testcompiler.h"
#include "testdesigner.h"
#include "testgraph.h"
#include "testprimitivesloader.h"

int main(int argc, char** argv){
    int status = 0;

    status |= QTest::qExec(new TestCompiler, argc, argv);

    status |= QTest::qExec(new TestDesigner, argc, argv);

    status |= QTest::qExec(new TestGraph, argc, argv);

    status |= QTest::qExec(new TestPrimitivesLoader, argc, argv);

    return status;
}
