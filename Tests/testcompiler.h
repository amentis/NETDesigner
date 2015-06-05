#ifndef TESTCOMPILER_H
#define TESTCOMPILER_H

#include <QtTest>
class Compiler;

class TestCompiler : public QObject
{
public:
    TestCompiler();
    Q_OBJECT
private:
    Compiler* compiler;
private slots:
    void initTestCase();

    void testCheckForErrors();
    void testTranslate();
    void testCompile();
    void testClean();
    void testBuild();
};

#endif // TESTCOMPILER_H
