#include "testprimitivesloader.h"

#include <QtTest>

#include "primitivesloader.h"
#include "base.h"
#include "primitive.h"

TestPrimitivesLoader::TestPrimitivesLoader(){}

TestPrimitivesLoader::~TestPrimitivesLoader()
{
    delete primitivesLoader;
    delete infoFileString;
    delete srcFileString;
    delete baseDir;
}

void TestPrimitivesLoader::initTestCase()
{
#ifdef Q_OS_WIN
    QLibrary libPrimitivesLoader("PrimitivesLoader.dll");
#else
    QLibrary libPrimitivesLoader("libPrimitivesLoader");
#endif
    libPrimitivesLoader.load();
    if (!libPrimitivesLoader.isLoaded()){
        qCritical("Cannot load libPrimitivesLoader");
        qApp->exit(qApp->exec());
    }
    primitivesLoader = new PrimitivesLoader();

    infoFileString = new QString("a: \n b: int a, int b \n c: int a, real b, bool c, string d \n");
    srcFileString = new QString("namespace Test{\n bool a(){}\n bool b(int a, int b){}\n"
                                "bool c(int a, float b, bool c, char d){} };\n");
    baseDir = new QDir(QDir::temp());
    baseDir->mkdir("Test");
    baseDir->cd("Test");
    QFile infoFile(baseDir->absolutePath() + "/Test.info");
    infoFile.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly);
    QTextStream infoStream(&infoFile);
    infoStream << *infoFileString;
    infoFile.close();
    QFile srcFile(baseDir->absolutePath() + "/Test.cpp");
    srcFile.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly);
    QTextStream srcStream(&srcFile);
    srcStream << *srcFileString;
    srcFile.close();
}

void TestPrimitivesLoader::testLoadBase()
{
    Base* base = primitivesLoader->loadBase(new QString("Test"), new QString(baseDir->absoluteFilePath("Test.info")), new QString(baseDir->absoluteFilePath("Test.cpp")));
    QCOMPARE(*(base->getName()), QString("Test"));
    QCOMPARE(base->primitives()->size(), 3);

    QFETCH(int, i);
    QFETCH(QString, definition);

    QCOMPARE(*(base->primitives()->at(i)->definition()), definition);
}

void TestPrimitivesLoader::testLoadBase_data()
{
    QTest::addColumn<int>("i");
    QTest::addColumn<QString>("definition");

    QTest::newRow("primitive 1") << 0 << "a()";
    QTest::newRow("primitive 2") << 1 << "b(int a, int b)";
    QTest::newRow("primitive 3") << 2 << "c(int a, real b, bool c, string d)";
}

void TestPrimitivesLoader::testSrcFile()
{
    QCOMPARE(*(primitivesLoader->srcFile(new QString("Test"))), QString(baseDir->absoluteFilePath("Test.cpp")));
}

void TestPrimitivesLoader::testFindPrimitiveByName()
{
    Base* base = primitivesLoader->loadBase(new QString("Test"), new QString(baseDir->absoluteFilePath("Test.info")), new QString(baseDir->absoluteFilePath("Test.cpp")));

    QVector<Base*> bases;
    bases.append(base);

    QFETCH(QString, name);
    QFETCH(QString, definition);

    QCOMPARE(*(primitivesLoader->findPrimitiveByName(&bases, &name)->definition()), definition);
}

void TestPrimitivesLoader::testFindPrimitiveByName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("definition");

    QTest::newRow("primitive 1") << "a" << "a()";
    QTest::newRow("primitive 2") << "b" << "b(int a, int b)";
    QTest::newRow("primitive 3") << "c" << "c(int a, real b, bool c, string d)";
}
