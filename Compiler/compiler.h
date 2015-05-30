#ifndef COMPILER_H
#define COMPILER_H

#include "compiler_global.h"

#include <QVector>
#include <QStack>
#include <QHash>

class QTextStream;
class QString;
class QDir;
class QStringList;

class Graph;
class Node;
class Arrow;

class Base;

class COMPILERSHARED_EXPORT Compiler
{

public:
    Compiler();
    Compiler(QVector<Base*>* bases, QString* projectPath, QString* mainGraph);
    ~Compiler();
    bool checkForErrors(QTextStream& output);
    bool translate(QTextStream& output);
    bool compile(QTextStream& output);
    void clean(QTextStream& output, bool binary = false);
    bool build(QTextStream& output);

private:
    bool checkGraphForErrors(Graph* graph, QTextStream& output);
    bool generateProject(QTextStream& output);
    bool generateNet(Graph* net, QTextStream& output);
    bool generateNode(Node* node, Graph* graph,QTextStream& output);
    bool generateArrow(Arrow* arrow, Graph* graph, QTextStream& output);
    bool generateVars(QTextStream& output);
    bool calculateVars(Graph* net, QTextStream& output);

    enum class VarType {Int, Real, String, Bool};

    QVector<Graph*>* mGraphs;
    QVector<QString>* mGraphNames;
    QVector<Base*>* mBases;
    QHash<QString, QHash<QString, VarType>*>* vars;
    QString* projectName;
    QString* mMainGraph;
    QStack<QString>* includes;

    QDir* srcDir;
    QDir* binDir;
};

#endif // COMPILER_H
