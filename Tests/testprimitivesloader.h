#ifndef TESTPRIMITIVESLOADER_H
#define TESTPRIMITIVESLOADER_H

#include <QtTest>

class PrimitivesLoader;

class TestPrimitivesLoader : public QObject
{
    Q_OBJECT
public:
    TestPrimitivesLoader();

private:
    PrimitivesLoader* primitivesLoader;

private slots:
    void initTestCase();

    void testLoadBase();
    void testSrcFile();
    void testFindPrimitiveByName();

};

#endif // TESTPRIMITIVESLOADER_H
