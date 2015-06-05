#include "testdesigner.h"

#include "addarrowexpressiondialog.h"
#include "addnode.h"
#include "browseprimitivesdialog.h"
#include "editarrowdialog.h"
#include "programoptionsdialog.h"
#include "editor.h"
#include "mainwindow.h"


TestDesigner::TestDesigner(){}

void TestDesigner::initTestCase()
{
    int argc = 0;
    char **argv = new char*;
    QApplication app(argc, argv);
    addNode = new AddNode();
    mainWindow = new MainWindow();
}

void TestDesigner::testAddArrowExpressionDialog()
{
    //TODO
}

void TestDesigner::testAddNode()
{
    //TODO
}

void TestDesigner::testBrowsePrimitivesDialog()
{
    //TODO
}

void TestDesigner::testEditArrowDialog()
{
    //TODO
}

void TestDesigner::testProgramOptionsDialog()
{
    //TODO
}

void TestDesigner::testIsModified()
{
    //TODO
}

void TestDesigner::testSaveLoad()
{
    //TODO
}

void TestDesigner::testCreateEditDeleteNodeArrow()
{
    //TODO
}

void TestDesigner::testCreateProject()
{
    //TODO
}

void TestDesigner::testOpenProject()
{
    //TODO
}

void TestDesigner::testCloseProject()
{
    //TODO
}

void TestDesigner::testSaveProject()
{
    //TODO
}

void TestDesigner::testAddNet()
{
    //TODO
}

void TestDesigner::testRemoveNet()
{
    //TODO
}

void TestDesigner::testExit()
{
    //TODO
}
