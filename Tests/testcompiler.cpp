#include "testcompiler.h"
#include "compiler.h"


TestCompiler::TestCompiler(){}

void TestCompiler::initTestCase()
{
#ifdef Q_OS_WIN
    QLibrary libCompiler("Compiler.dll");
#else
    QLibrary libCompiler("libCompiler");
#endif
    libCompiler.load();
    if (!libCompiler.isLoaded()){
        qCritical("Cannot load libCompiler");
        qApp->exit(qApp->exec());
    }
    compiler = new Compiler();
}

void TestCompiler::testCheckForErrors()
{
    //TODO
}

void TestCompiler::testTranslate()
{
    //TODO
}

void TestCompiler::testCompile()
{
    //TODO
}

void TestCompiler::testClean()
{
    //TODO
}

void TestCompiler::testBuild()
{
    //TODO
}
