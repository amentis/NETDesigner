#ifndef TESTCOMPILER_H
#define TESTCOMPILER_H

#include <QObject>

class QDir;
template<typename T>class QVector;
class QString;

class Compiler;

class Base;

class TestCompiler : public QObject
{
public:
    TestCompiler();
    Q_OBJECT
private:
    Compiler* compiler;
    QDir* projectDir;
    QVector<Base*>* bases;
    QString* mainNetName;
private slots:
    void initTestCase();

    void testCheckForErrors();
    void testTranslate();
    void testCompile();
    void testClean();
    void testBuild();
};

#endif // TESTCOMPILER_H
