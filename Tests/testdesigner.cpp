#include "testdesigner.h"

#include "addarrowexpressiondialog.h"
#include "addnode.h"
#include "browseprimitivesdialog.h"
#include "editarrowdialog.h"
#include "programoptionsdialog.h"
#include "editor.h"
#include "mainwindow.h"

#include <QtTest>
#include <QtWidgets>

TestDesigner::TestDesigner(){}

void TestDesigner::testAddArrowExpressionDialog()
{
    int argc = 0;
    char **argv = new char*;
    QApplication app(argc, argv);
    addArrowExpressionDialog = new AddArrowExpressionDialog(true);
    addArrowExpressionDialog->show();
    QTest::keyClick(addArrowExpressionDialog, Qt::Key::Key_3);

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
