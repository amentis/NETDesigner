#ifndef COMPILER_H
#define COMPILER_H

#include "compiler_global.h"

#include <QVector>

class QTextStream;
class QString;
class QDir;

class Graph;

class Base;

class COMPILERSHARED_EXPORT Compiler
{

public:
    Compiler();
    Compiler(QVector<Base*>* bases, QString* projectPath);
    ~Compiler();
    bool checkForErrors(QTextStream& output);
    bool translate(QTextStream& output);
    bool compile(QTextStream& output);
    bool build(QTextStream& output);

private:
    bool checkGraphForErrors(Graph* graph, QTextStream& output);
    QVector<Graph*>* mGraphs;
    QVector<QString>* mGraphNames;
    QVector<Base*>* mBases;
    QDir* srcDir;
    QDir* binDir;
};

#endif // COMPILER_H
