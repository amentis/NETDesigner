#include "testcompiler.h"
#include "compiler.h"

#include <QtTest>

#include "base.h"

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

    projectDir = new QDir(QDir::temp());
    projectDir->mkdir("TestProject");
    projectDir->cd("TestProject");

    bases = new QVector<Base*>();
}

void TestCompiler::testCheckForErrors()
{
    mainNetName = new QString("");
    compiler = new Compiler(bases, new QString(projectDir->absolutePath()), mainNetName);
    QString str;
    QTextStream stream(&str);
    QCOMPARE(compiler->checkForErrors(stream), false);

    delete compiler;
    delete mainNetName;
    QFile projectFile(projectDir->absolutePath() + "/net.project");
    projectFile.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly);
    QTextStream fstr(&projectFile);
    fstr << "Net\n" << "0:0:0:0:1:0:1:0\n";
    projectFile.close();
    QFile netFile(projectDir->absolutePath() + "/Net.net");
    netFile.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly);
    netFile.close();

    mainNetName = new QString("Net");

    compiler = new Compiler(bases, new QString(projectDir->absolutePath()), mainNetName);

    QCOMPARE(compiler->checkForErrors(stream), false);

    netFile.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly);
    fstr.setDevice(&netFile);
    fstr << "0:3::80,91;1:4::423,119;\n" << "0:1: :x:0:0:1:;";
    netFile.close();

    QCOMPARE(compiler->checkForErrors(stream), true);
}

void TestCompiler::testTranslate()
{
    QString str;
    QTextStream stream(&str);
    QCOMPARE(compiler->translate(stream), true);
}

void TestCompiler::testCompile()
{
    QString str;
    QTextStream stream(&str);
    QCOMPARE(compiler->compile(stream), true);
    QDir buildDir(*projectDir);
    buildDir.cd("build");
#ifdef Q_OS_WIN
    QCOMPARE(buildDir.exists("TestProject.exe"), true);
#else
    QCOMPARE(buildDir.exists("TestProject"), true);
#endif

}

void TestCompiler::testClean()
{
    QString str;
    QTextStream stream(&str);
    compiler->clean(stream);
    QDir buildDir(*projectDir);
    buildDir.cd("build");
#ifdef Q_OS_WIN
    QCOMPARE(buildDir.exists("TestProject.exe"), false);
#else
    QCOMPARE(buildDir.exists("TestProject"), false);
#endif
}

void TestCompiler::testBuild()
{
    QString str;
    QTextStream stream(&str);
    QCOMPARE(compiler->build(stream), true);
    QDir buildDir(*projectDir);
    buildDir.cd("build");
#ifdef Q_OS_WIN
    QCOMPARE(buildDir.exists("TestProject.exe"), true);
#else
    QCOMPARE(buildDir.exists("TestProject"), true);
#endif
}
