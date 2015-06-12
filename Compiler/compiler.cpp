#include "compiler.h"

#include <QtCore>

#include "base.h"
#include "primitive.h"

#include "graph.h"
#include "arrow.h"

Compiler::Compiler()
{
#ifdef Q_OS_WIN
    QLibrary libGraph("Graph.dll");
#else
    QLibrary libGraph("libGraph");
#endif
    libGraph.load();
    if (!libGraph.isLoaded()){
        qCritical("Cannot load libGraph");
        qApp->exit(qApp->exec());
    }
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
}

Compiler::Compiler(QVector<Base*>* bases, QString* projectPath, QString* mainGraph) : Compiler()
{
    mBases = bases;
    srcDir = new QDir(*projectPath);
    projectName = new QString(srcDir->dirName());
    binDir = new QDir(*projectPath + "/build");
    binDir->mkpath(".");

    mGraphs = new QVector<Graph*>();
    mGraphNames = new QVector<QString>();
    mMainGraph = mainGraph;

    vars = new QHash<QString,QHash<QString, VarType>*>();

    QStringList filters;
    filters.append("*.net");
    QFileInfoList files = srcDir->entryInfoList(filters);
    for (const auto& fileInfo : files){
        Graph* tmp = new Graph();
        QFile file (fileInfo.absoluteFilePath());
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        basesFailedLoading = new QStringList();
        if (tmp->loadFromStream(stream, mBases)){
            mGraphNames->append(fileInfo.baseName());
            mGraphs->append(tmp);
        } else {
            basesFailedLoading->append(fileInfo.fileName());
        }
        file.close();
    }
}

Compiler::~Compiler()
{

}

bool Compiler::checkForErrors(QTextStream &output)
{
    if (!srcDir->exists()){
        output << "[Critical] Source directory does not exist! \n";
        return false;
    }

    if (!binDir->exists()){
        output << "[Critical] Could not create build dirctory " << binDir->absolutePath() << "!\n";
        return false;
    }

    if (!basesFailedLoading->isEmpty()){
        for (const auto& str : *basesFailedLoading)
            output << "[Error] Failed loading base " << str << "! Check if base is valid!\n";
        return false;
    }

    bool fail = false;

    if (mMainGraph->remove(" ") == ""){
        output << "[Error] No main net selected!\n";
        return false;
    }

    QDir primitivesDir(srcDir->absolutePath() + "/Primitives");
    QFileInfoList files = primitivesDir.entryInfoList();
    for (const auto& base : *mBases){
        bool found = false;
        for (const auto& info : files){
            if (QString(*(base->getName())) == info.fileName()){
                found = true;
                break;
            }
        }
        if (!found){
            output << "[Error] Cannot find primitives base: " << *(base->getName()) << "!\n";
            fail = true;
        }
    }

    if (fail){
        output << "Failed to compile due to errors in code.\n";
        return false;
    }

    if (mGraphs->isEmpty()){
        output << "[Error] No graphs to compile!\n";
        return false;
    }

    for (const auto& graph : *mGraphs){
        if (!checkGraphForErrors(graph, output))
            fail = true;
    }

    if (fail){
        output << "Failed to compile due to errors in code. \n";
        return false;
    }
    return true;

}

bool Compiler::translate(QTextStream &output)
{
    if (!binDir){
        output << "[Critical] binary directory not initialized! Writable? \n";
        return false;
    }
    includes = new QStack<QString>();
    for (const auto& graph : *mGraphs){
        if (generateNet(graph, output)){
            for (const auto& node : *(graph->nodes())){
                if (!generateNode(node, graph, output))
                    return false;
            }
            for (const auto& arrow : *(graph->arrows())){
                if (!generateArrow(arrow, graph, output))
                    return false;
            }
        } else
            return false;
    }
        if (!generateProject(output))
            return false;
        return generateVars(output);
}

bool Compiler::compile(QTextStream &output)
{
#ifdef Q_OS_WIN
    QString executable(qApp->applicationDirPath() + "/clang/bin/clang++.exe");
#else
    QString executable("/bin/clang++");
    if (!QFile(executable).exists())
        executable = "/usr/bin/clang++";
#endif
    if (!QFile(executable).exists()){
        output << "[Critical] Cannot find " << executable << "!\n";
        return false;
    }
    QString inputFile(binDir->absolutePath() + "/" + *(projectName) + ".cpp");
    QString outputFile(binDir->absolutePath() + "/" + *(projectName));

#ifdef Q_OS_WIN
    outputFile.append(".exe");
#endif

    QStringList args;

#ifdef Q_OS_WIN
    args << "-target=i686-w64-mingw32";
#endif

    args << "-I" << binDir->absolutePath() << inputFile << "-o" << outputFile;

    QProcess clang;

    clang.setProcessChannelMode(QProcess::MergedChannels);

    clang.start(executable, args);

    if (!clang.waitForStarted()){
        output << "[Critical] Failed to start process " << executable << "!\n";
        return false;
    }

    QByteArray data;

    while(clang.waitForReadyRead())
        data.append(clang.readAllStandardOutput());

    output << data;

    if (!QFile(outputFile).exists()){
        output << "[Critical] Compile failed at executing C++ compiler!\n";
        return false;
    }
    return true;
}

void Compiler::clean(QTextStream& output, bool binary)
{
    bool fail = false;
    for (const auto& info : binDir->entryInfoList(QDir::Files)){
        if (binary || (info.suffix() == "cpp" || info.suffix() == "h")){
            QFile file(info.absoluteFilePath());
            if (!file.remove()){
                fail = true;
                output << "[Cleanup Warning] Failed to delete file !\n";
                output << info.absolutePath();
            }
        }
    }
    if (binary && !fail){
        output << "[Clean] Success\n";
    } else if (binary && fail){
        output << "[Clean] Fail\n";
    }
}

bool Compiler::build(QTextStream &output)
{
    if (checkForErrors(output)){
            if (translate(output)){
                if (compile(output)){
                    clean(output);
                    output << "[Build] Success \n";
                    return true;
                }
            }
    }
    output << "[Build] Fail \n";
    return false;
}

bool Compiler::checkGraphForErrors(Graph* graph, QTextStream& output)
{
    bool fail = false;

    if (!graph->hasStartNode()){
        output << "[Critical] Graph has no start node! \n";
        return false;
    }
    if (!graph->hasEndNode()){
        output << "[Critical] Graph has no end nodes! \n";
        return false;
    }

    bool startNodeEncountered = false;
    for (const auto& node : *graph->nodes()){
        if (node->type() == Node::NodeType::StartNode){
            if (startNodeEncountered){
                output << "[Error] Graph contains several start nodes! \n";
            fail = true;
            }
            else
                startNodeEncountered = true;

            if (!node->arrowsIn()->isEmpty()){
                output << "[Error] Start node contains incomming arrows! \n";
                fail = true;
            }

            if (node->arrowsOut()->isEmpty()){
                output << "[Error] Start node has no outgoing arrows! \n";
            }

        } else if (node->type() == Node::NodeType::EndNode){
            if (!node->arrowsOut()->isEmpty())
                output << "[Warning] End node at position" << node->position()->x() << "x" << node->position()->y() <<
                            " has outgoing arrows. They will be ignored. \n";


        } else {
            if (node->arrowsIn()->isEmpty())
                output << "[Warning] Node at position " << node->position()->x() << "x" << node->position()->y() <<
                            " has no incoming arrows. The node and all arrows starting from it will be ignored. \n";
            if (node->arrowsOut()->isEmpty()){
                output << "[Error] Node at position " << node->position()->x() << "x" << node->position()->y() <<
                          " has no outgoing arrows! Did you intend it to be an end node? \n";
                fail = true;
            }
        }
    }

    for (const auto& arrow : *graph->arrows()){
        bool ignored = false;
        if (!arrow->from() || !arrow->to()){
            output << "[Warning] Arrow leading from no node to no node will be ignored. \n";
            ignored = true;
        } else if (!arrow->from()){
            output << "[Warning] Arrow leading from no node to node at position " << arrow->to()->position()->x() << "x" <<
                        arrow->to()->position()->y() << " will be ignored. \n";
            ignored = true;
        } else if (!arrow->to()){
            output << "[Error] Arrow coming from node at position" << arrow->from()->position()->x() << "x" <<
                      arrow->from()->position()->y() << " does not lead to node! \n";
            fail = true;
        }

        if (!ignored || !fail){
            for (const auto& primitive : *arrow->primitives()){
                if (!mBases->contains(primitive->base())){
                    output << "[Error] Base " << primitive->base()->getName() << " not included in project! Required by primitive " <<
                              primitive->name() << " used by arrow from " << arrow->from()->position()->x() << "x" <<
                              arrow->from()->position()->y() << " to " << arrow->to()->position()->x() << "x" <<
                              arrow->to()->position()->y() << ". \n";
                    fail = true;
                } else {
                    if (!primitive->base()->primitives()->contains(primitive)){
                        output << "[Error] Base " << primitive->base()->getName() << " does not contain the primitive " <<
                                  primitive->name();
                        output << "\n";
                        fail = true;
                    }
                }
            }
        }

    }
    if (fail){
        output << "Compile stopped due to previous errors. \n";
        return false;
    }
    return true;
}

bool Compiler::generateProject(QTextStream& output)
{
    QFile projectFile(binDir->absolutePath().append("/").append(*projectName).append(".cpp"));
    if (!projectFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        output << "[Critical] cannot open %1 for writing! \n", binDir->absolutePath().append(*projectName).append(".cpp");
        return false;
    }

    for (const auto& base : *mBases)
            includes->append("../Primitives/" + *(base->getName()) + "/" + *(base->getName()) + ".cpp");
    includes->append("vars.h");

    QTextStream stream(&projectFile);

#ifdef Q_OS_WIN
    stream << "#include <stdlib.h>\n";
#endif

    while (!includes->isEmpty()){
        stream << "#include \"";
        stream << includes->pop();
        stream << "\"\n";
    }
    stream << "\nint main (int argc, char** argv){ \n";
#ifdef Q_OS_WIN
    stream << "    bool result = ";
    stream << *mMainGraph;
    stream << "_net";
    stream << "();\n";
    stream << "    system(\"pause\");";
    stream << "    return (int) result;\n";
#else
    stream << "    return (int) (!";
    stream << *mMainGraph;
    stream << "_net";
    stream << "());\n";
#endif
    stream << "}\n";
    projectFile.close();
    return true;
}

bool Compiler::generateNet(Graph* net, QTextStream& output)
{
    QString name = mGraphNames->at(mGraphs->indexOf(net));

    vars->insert(name, new QHash<QString, VarType>());

    calculateVars(net, output);

    name.append(".cpp");
    QString path = binDir->absolutePath().append("/").append(name);
    QFile netFile(path);
    if (!netFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        output << "[Critical] cannot open ";
        output << path;
        output << "for writing! \n";
        return false;
    }
    QTextStream stream(&netFile);
    stream << "#include \"vars.h\"\n";
    stream << "\nbool ";
    stream << mGraphNames->at(mGraphs->indexOf(net));
    stream << "_net(){\n";
    stream << "return ";
    stream << mGraphNames->at(mGraphs->indexOf(net));
    stream << "_node";
    stream << net->internalNodeIndex(net->startNode());
    stream << "();\n";
    stream << "}\n";
    netFile.close();
    includes->append(name);
    return true;
}

bool Compiler::generateNode(Node* node, Graph* graph, QTextStream& output)
{
    QString fileName(mGraphNames->at(mGraphs->indexOf(graph))
                     + "_node" + QString::number(graph->internalNodeIndex(node)));

    QFile file(binDir->absolutePath() + "/" + fileName + ".cpp");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        output << "[Critical] Cannot write " + binDir->absolutePath() + "/" + fileName + ".cpp" + "!\n";
        return false;
    }
    QTextStream stream(&file);
    stream << "typedef bool (*arrow)(bool);\n\n";
    stream << "#include \"vars.h\"\n\n";
    if (node->type() == Node::NodeType::ProximityNode && node->expression()->isEmpty()){
        output << "[Error] Empty proximity node expression!";
        return false;
    }
    if (node->type() == Node::NodeType::CaseNode || node->type() == Node::NodeType::ProximityNode){
        stream << "#include <math.h>\n#include <limits.h>\n\n";
        stream << "using namespace ";
        stream << mGraphNames->at(mGraphs->indexOf(graph));
        stream << ";\n\n";
    }

    stream << "bool ";
    stream << mGraphNames->at(mGraphs->indexOf(graph));
    stream << "_node";
    stream << graph->internalNodeIndex(node);
    stream << "(){\n";

    stream << "    unsigned numArrows = ";
    stream << node->arrowsOut()->size();
    stream << ";\n";

    if (node->type() == Node::NodeType::CaseNode || node->type() == Node::NodeType::ProximityNode){
        stream << "    double res = ";
        stream << *(node->expression());
        stream << ";\n";

        stream << "    double consts[] =  {";
        bool first = true;
        for (const auto& arrow : *(node->arrowsOut())){
            if (first){
                first = false;
            } else {
                stream << ", ";
            }
            stream << *(arrow->expression());
        }
        stream << "};\n\n";

        stream << "    int arrIndex = 0;\n\n";

        stream << "    arrow unsorted[] = {";

        first = true;
        for (const auto& arrow : *(node->arrowsOut())){
            if (first){
                first = false;
            } else {
                stream << ", ";
            }

            stream << "&";
            stream << mGraphNames->at(mGraphs->indexOf(graph));
            stream << "_a";
            stream << graph->internalNodeIndex(node);
            stream << graph->internalNodeIndex(arrow->to());
        }
        if (node->arrowsOut()->size() == 1)
            stream << ", 0";
        stream << "};\n";

        if (node->arrowsOut()->size() == 1){
            stream << "    arrow arrs[2];\n";
            stream << "    arrs[1] = 0;\n";
        } else {
            stream << "    arrow arrs[";
            stream << node->arrowsOut()->size();
            stream << "];\n";
        }

        stream << "    for (int i = 0; i < ";
        stream << node->arrowsOut()->size();
        stream << "; ++i){\n";
        stream << "        double distance = fabs(consts[0] - res);\n";
        stream << "        int idx = 0;\n";
        stream << "        for (int c = 1; c < ";
        stream << node->arrowsOut()->size();
        stream << "; ++c){\n";
        stream << "            double cdistance = fabs(consts[c] - res);\n";
        stream << "            if (cdistance < distance){\n";
        stream << "                idx = c;\n";
        stream << "                distance = cdistance;\n";
        stream << "            }\n        }\n";
        stream << "        arrs[arrIndex] = unsorted[idx];\n";
        stream << "        arrIndex++;\n";
        stream << "        consts[idx] = INT_MAX;\n";
        stream << "    }\n";
    } else {
        stream << "    arrow arrs[] = {";

        bool first = true;
        for (const auto& arrow : *(node->arrowsOut())){
            if (first){
                first = false;
            } else {
                stream << ", ";
            }

            stream << "&";
            stream << mGraphNames->at(mGraphs->indexOf(graph));
            stream << "_a";
            stream << graph->internalNodeIndex(node);
            stream << graph->internalNodeIndex(arrow->to());
        }
        if (node->arrowsOut()->size() == 1)
            stream << ", 0";
        stream << "};\n";
    }

    stream << "    bool fail = false;\n";
    stream << "    for (unsigned short i = 0; i < ";
    stream << node->arrowsOut()->size();
    stream << "; ++i){\n";
    stream << "        if (arrs[i](true))\n";
    stream << "            break;\n";
    stream << "        else\n";
    stream << "            fail = true;\n    }\n";
    stream << "    return !fail;\n}\n";

    file.close();
    includes->append(fileName + ".cpp");
    return true;
}

bool Compiler::generateArrow(Arrow* arrow, Graph* graph, QTextStream& output)
{
    QString fileName(mGraphNames->at(mGraphs->indexOf(graph))
                     + "_a" + QString::number(graph->internalNodeIndex(arrow->from())) +
                     QString::number(graph->internalNodeIndex(arrow->to())));

    QFile file(binDir->absolutePath() + "/" + fileName + ".cpp");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        output << "[Critical] Cannot write " + binDir->absolutePath() + "/" + fileName + ".cpp" + "!\n";
        return false;
    }
    QTextStream stream(&file);

    stream << "bool ";
    stream << mGraphNames->at(mGraphs->indexOf(graph));
    stream << "_node";
    stream << graph->internalNodeIndex(arrow->to());
    stream << "();\n\n";

    QString* subnetCalled = nullptr;
    if (arrow->subnetCalled(subnetCalled)){
        stream << "bool ";
        stream << *(subnetCalled);
        stream << "_net();\n\n";
    }

    stream << "#include \"vars.h\"\n\n";
    stream << "using namespace ";
    stream << mGraphNames->at(mGraphs->indexOf(graph));
    stream << ";\n\n";
    stream << "bool ";
    stream << fileName;
    stream << "(bool _FORW){\n";
    stream << "    FORW = _FORW;\n\n";
    stream << "    int progress = (FORW)? 0 : ";
    stream << (arrow->primitives()->size() - 1);
    stream << " ;\n\n";

    if (subnetCalled){
        stream << "    if (FORW)\n";
        stream << "        FORW = ";
        stream << *(subnetCalled);
        stream << "_net();\n\n";
    }

    for (const auto& primitive : *(arrow->primitives())){
        stream << "    if (FORW){\n";
        stream << "        if (";
        stream << *(primitive->base()->getName());
        stream << "::";
        stream << *(primitive->name());
        stream << "(";
        if (primitive->hasArguments())
            stream << *(arrow->argumentsForPrimitive(primitive->name()));
        stream << ")){\n";
        stream << "            progress ++;\n        } else {\n            FORW = false;\n        }\n    }\n";
    }

    stream << "    if (!FORW){\n        progress--;\n        switch(progress){\n";
    for (int i = arrow->primitives()->size() - 1; i >= 0; --i){
        stream << "            case ";
        stream << i;
        stream << " : ";
        stream << *(arrow->primitives()->at(i)->base()->getName());
        stream << "::";
        stream << *(arrow->primitives()->at(i)->name());
        stream << "(";
        if (arrow->primitives()->at(i)->hasArguments())
            stream << *(arrow->argumentsForPrimitive(arrow->primitives()->at(i)->name()));
        stream << ");\n";
    }
    stream << "        }\n        return false;\n    }\nreturn ";
    stream << mGraphNames->at(mGraphs->indexOf(graph));
    stream << "_node";
    stream << graph->internalNodeIndex(arrow->to());
    stream << "();\n}\n";
    file.close();
    includes->append(fileName + ".cpp");
    return true;
}

bool Compiler::generateVars(QTextStream& output)
{
    QFile file(binDir->absolutePath() + "/vars.h");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        output << "[Critical] Cannot write file: ";
        output << binDir->absolutePath() + "/vars.h";
        return false;
    }
    QTextStream stream(&file);
    stream << "#ifndef VARS_H\n";
    stream << "#define VARS_H\n\n";
    stream << "#include <string>\n\n";
    stream << "bool FORW=true;\n\n";
    for (const auto& graphKey : vars->keys()){
        stream << "namespace ";
        stream << graphKey;
        stream << "{\n";
        QHash<QString, VarType>* currentVars = vars->value(graphKey);
        for (const auto& var : currentVars->keys()){
            switch (currentVars->value(var)){
            case VarType::Bool: stream << "bool "; stream << var; stream << " = false;\n";break;
            case VarType::Int: stream << "int "; stream << var; stream << " = 0;\n";break;
            case VarType::Real: stream << "float "; stream << var; stream << " = .0f;\n"; break;
            case VarType::String: stream << "std::string "; stream << var; stream << " = \"\";\n";break;
            }
        }
        stream << "}\n\n";
    }
    stream << "#endif\n";
    file.close();
    return true;
}

bool Compiler::calculateVars(Graph* net, QTextStream& output)
{
    QHash<QString, VarType>* currentVars = vars->value(mGraphNames->at(mGraphs->indexOf(net)));
    for (const auto& arrow : *net->arrows()){
        for (const auto& primitive : *arrow->primitives()){
            QString name = *(primitive->name());
            QString primitiveArgsString =
                    primitive->definition()->split("(", QString::SkipEmptyParts)[1].remove(")");
            QStringList primitiveArgs = primitiveArgsString.split(",",QString::SkipEmptyParts);

            if (primitiveArgs.size() == 0)
                continue;

            QString arrowPrimitiveArgsString(*(arrow->argumentsForPrimitive(&name)));
            QStringList arrowPrimitiveArgs = arrowPrimitiveArgsString.split(",");

            if (primitiveArgs.size() != arrowPrimitiveArgs.size()){
                output << "[Error] Primitive ";
                output << name;
                output << "'s' arguments definition (";
                output << primitiveArgsString;
                output << ") does not match supplied: (";
                output << arrowPrimitiveArgsString;
                output << ")!\n";
                return false;
            }

            for (int i = 0; i<arrowPrimitiveArgs.size(); ++i){
                QString arrowPrimitiveArg = arrowPrimitiveArgs[i].remove(" ");
                QRegExp regExpD("\\d*");
                QRegExp regExpF("\\d+\\.?\\d*");
                if (regExpD.exactMatch(arrowPrimitiveArg) || regExpF.exactMatch(arrowPrimitiveArg)
                        || arrowPrimitiveArg.compare("true", Qt::CaseInsensitive) == 0
                        || arrowPrimitiveArg.compare("false", Qt::CaseInsensitive) == 0)
                    continue;
                QString primitiveArg = primitiveArgs[i];
                VarType argType;
                if (primitiveArg.contains("int", Qt::CaseInsensitive))
                    argType = VarType::Int;
                else if (primitiveArg.contains("real", Qt::CaseInsensitive))
                    argType = VarType::Real;
                else if (primitiveArg.contains("string", Qt::CaseInsensitive))
                    argType = VarType::String;
                else if (primitiveArg.contains("bool", Qt::CaseInsensitive))
                    argType = VarType::Bool;
                else {
                    output << "[Error] Cannot name type for argument ";
                    output << i;
                    output << "of primitive ";
                    output << name;
                    output << "!\n";
                    return false;
                }

                if (currentVars->contains(arrowPrimitiveArg)){
                    if (currentVars->value(arrowPrimitiveArg) != argType){
                        output << "[Error] Variable ";
                        output << arrowPrimitiveArg;
                        output << "cannot be declared both as both as ";
                        switch (currentVars->value(arrowPrimitiveArg)){
                        case VarType::Int: output << "Int"; break;
                        case VarType::Real: output << "Real"; break;
                        case VarType::String: output << "String"; break;
                        case VarType::Bool: output << "Boolean"; break;
                        }
                        output << " and ";
                        switch (argType){
                        case VarType::Int: output << "Int"; break;
                        case VarType::Real: output << "Real"; break;
                        case VarType::String: output << "String"; break;
                        case VarType::Bool: output << "Boolean"; break;
                        }
                        output << "!\n";
                        return false;
                    }
                } else {
                    currentVars->insert(arrowPrimitiveArg, argType);
                }
            }
        }
    }
    return true;
}

