#include "primitivesloader.h"

#include "base.h"
#include "primitive.h"

#include <QtCore>

PrimitivesLoader::PrimitivesLoader()
{
}

Base*PrimitivesLoader::loadBase(QString* name, QString* infoFileDir, QString* srcFile)
{
    Base* base = new Base(*name);

    QFile infoFile(*infoFileDir);

    infoFile.open(QFile::ReadOnly);

    QTextStream stream(&infoFile);


    while(!stream.atEnd()){
        QString str = stream.readLine();

        QStringList line = str.split(":");
        QString primitiveName = line[0].remove(" ");
        Primitive* primitive = new Primitive(base, new QString(primitiveName));
        QStringList arguments = line[1].split(",");
        for (const auto& argument : arguments){
            QString argType = argument.split(" ", QString::SkipEmptyParts)[0].toLower().remove("*").remove("&");
            QString* argName = new QString(argument.split(" ", QString::SkipEmptyParts)[1]);
            if (argType == "int"){
                primitive->addArgument(Primitive::ArgumentType::Int, argName);
            } else if (argType == "float" || argType == "real") {
                primitive->addArgument(Primitive::ArgumentType::Real, argName);
            } else if (argType == "bool" || argType == "boolean") {
                primitive->addArgument(Primitive::ArgumentType::Bool, argName);
            } else if (argType == "string" || argType == "char") {
                primitive->addArgument(Primitive::ArgumentType::String, argName);
            } else {
                qCritical("Critical error loading primitive %s : invalid argument type %s!",
                          str.toLocal8Bit().constData(), argType.toLocal8Bit().constData());
                return nullptr;
            }
        }
    }

    infoFile.close();

    mSrcFiles.insert(*name, srcFile);
    return base;
}

QString*PrimitivesLoader::srcFile(QString* baseName)
{
    return mSrcFiles.value(*baseName);
}
