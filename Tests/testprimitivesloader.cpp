#include "testprimitivesloader.h"

#include "primitivesloader.h"
#include "base.h"
#include "primitive.h"


TestPrimitivesLoader::TestPrimitivesLoader(){}

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
}

void TestPrimitivesLoader::testLoadBase()
{
    //TODO
}

void TestPrimitivesLoader::testSrcFile()
{
    //TODO
}

void TestPrimitivesLoader::testFindPrimitiveByName()
{
    //TODO
}
