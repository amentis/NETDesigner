#ifndef TESTPRIMITIVESLOADER_H
#define TESTPRIMITIVESLOADER_H

#include <QObject>

class QString;
class QDir;

class PrimitivesLoader;

class TestPrimitivesLoader : public QObject
{
    Q_OBJECT
public:
    TestPrimitivesLoader();
    ~TestPrimitivesLoader();

private:
    PrimitivesLoader* primitivesLoader;
    QString* infoFileString;
    QString* srcFileString;
    QDir* baseDir;

private slots:
    void initTestCase();

    void testLoadBase();
    void testLoadBase_data();
    void testSrcFile();
    void testFindPrimitiveByName();
    void testFindPrimitiveByName_data();

};

#endif // TESTPRIMITIVESLOADER_H
