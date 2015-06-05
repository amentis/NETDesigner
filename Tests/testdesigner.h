#ifndef TESTDESIGNER_H
#define TESTDESIGNER_H

#include <QtTest>

class AddArrowExpressionDialog;
class AddNode;
class BrowsePrimitivesDialog;
class EditArrowDialog;
class ProgramOptionsDialog;
class Editor;
class MainWindow;

class TestDesigner : public QObject
{
    Q_OBJECT
public:
    TestDesigner();
private:
    AddArrowExpressionDialog* addArrowExpressionDialog;
    AddNode* addNode;
    BrowsePrimitivesDialog* browsePrimitivesDialog;
    EditArrowDialog* editArrowDialog;
    ProgramOptionsDialog* programOptionsDialog;
    Editor* editor;
    MainWindow* mainWindow;

private slots:
    void initTestCase();

    void testAddArrowExpressionDialog();
    void testAddNode();
    void testBrowsePrimitivesDialog();
    void testEditArrowDialog();
    void testProgramOptionsDialog();

    //Editor
    void testIsModified();
    void testSaveLoad();
    void testCreateEditDeleteNodeArrow();

    //MainWindow
    void testCreateProject();
    void testOpenProject();
    void testCloseProject();
    void testSaveProject();
    void testAddNet();
    void testRemoveNet();
    void testExit();
};

#endif // TESTDESIGNER_H
