#include "compiler.h"

#include <QtCore>

#include "base.h"
#include "primitive.h"

#include "graph.h"
#include "arrow.h"

Compiler::Compiler()
{
    QLibrary libGraph("libGraph");
    libGraph.load();
    if (!libGraph.isLoaded()){
        qCritical("Cannot load libGraph");
        qApp->exit(qApp->exec());
    }

    QLibrary libPrimitivesLoader("libPrimitivesLoader");
    libPrimitivesLoader.load();
    if (!libPrimitivesLoader.isLoaded()){
        qCritical("Cannot load libPrimitivesLoader");
        qApp->exit(qApp->exec());
    }
}

Compiler::Compiler(QVector<Base*>* bases, QString* projectPath) : Compiler()
{
    mBases = bases;
    srcDir = new QDir(*projectPath);
    binDir = new QDir(*projectPath + "/build");
    binDir->mkdir(".");

    mGraphs = new QVector<Graph*>();
    mGraphNames = new QVector<QString>();


    QStringList filters;
    filters.append("*.net");
    QFileInfoList files = srcDir->entryInfoList(filters);
    for (QFileInfo fileInfo : files){
        Graph* tmp = new Graph();
        QFile file (fileInfo.absoluteFilePath());
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        if (tmp->loadFromStream(stream)){
            mGraphNames->append(fileInfo.fileName());
            mGraphs->append(tmp);
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
        output << "[Critical] Source directory does not exist!";
        return false;
    }

    if (!binDir->exists()){
        output << "[Critical] Could not create build dirctory " << binDir->absolutePath() << "!";
        return false;
    }

    bool fail = false;

    QFileInfoList files = srcDir->entryInfoList();
    for (const auto& base : *mBases){
        bool found = false;
        for (const auto& info : files){
            if (QString(*(base->getName())).append(".netpb") == info.fileName()){
                found = true;
                break;
            }
        }
        if (!found){
            output << "[Error] Cannot find primitives base: " << base->getName() << "!";
            fail = true;
        }
    }

    if (fail){
        output << "Failed to compile due to errors in code.";
        return false;
    }

    for (Graph* graph : *mGraphs){
        output << "Checking net for errors: " << mGraphNames->at(mGraphs->indexOf(graph));
        if (!checkGraphForErrors(graph, output))
            fail = true;
    }

    if (fail){
        output << "Failed to compile due to errors in code.";
        return false;
    }
    return true;

}

bool Compiler::translate(QTextStream &output)
{

}

bool Compiler::compile(QTextStream &output)
{

}

bool Compiler::build(QTextStream &output)
{
    if (checkForErrors(output)){
            if (translate(output)){
                if (compile(output)){
                    return true;
                }
            }
    }
    return false;
}

bool Compiler::checkGraphForErrors(Graph* graph, QTextStream& output)
{
    bool fail = false;

    if (!graph->hasStartNode()){
        output << "[Critical] Graph has no start node!";
        return false;
    }
    if (!graph->hasEndNode()){
        output << "[Critical] Graph has no end nodes!";
        return false;
    }

    bool startNodeEncountered = false;
    for (const auto& node : *graph->nodes()){
        if (node->type() == Node::NodeType::StartNode){
            if (startNodeEncountered){
                output << "[Error] Graph contains several start nodes!";
            fail = true;
            }
            else
                startNodeEncountered = true;

            if (!node->arrowsIn()->isEmpty()){
                output << "[Error] Start node contains incomming arrows!";
                fail = true;
            }

            if (!node->arrowsOut()->isEmpty()){
                output << "[Error] Start node has no outgoing arrows!";
            }

        } else if (node->type() == Node::NodeType::EndNode){
            if (!node->arrowsOut()->isEmpty())
                output << "[Warning] End node at position" << node->position()->x() << "x" << node->position()->y() <<
                            " has outgoing arrows. They will be ignored.";


        } else {
            if (node->arrowsIn()->isEmpty())
                output << "[Warning] Node at position " << node->position()->x() << "x" << node->position()->y() <<
                            " has no ingoing arrows. The node and all arrows starting from it will be ignored.";
            if (node->arrowsOut()->isEmpty()){
                output << "[Error] Node at position " << node->position()->x() << "x" << node->position()->y() <<
                          " has no outgoing arrows! Did you intend it to be an end node?";
                fail = true;
            }
        }
    }

    for (const auto& arrow : *graph->arrows()){
        bool ignored = false;
        if (!arrow->from() || !arrow->to()){
            output << "[Warning] Arrow leading from no node to no node will be ignored.";
            ignored = true;
        } else if (!arrow->from()){
            output << "[Warning] Arrow leading from no node to node at position " << arrow->to()->position()->x() << "x" <<
                        arrow->to()->position()->y() << " will be ignored.";
            ignored = true;
        } else if (!arrow->to()){
            output << "[Error] Arrow coming from node at position" << arrow->from()->position()->x() << "x" <<
                      arrow->from()->position()->y() << " does not lead to node!";
            fail = true;
        }

        if (!ignored || !fail){
            for (const auto& primitive : *arrow->primitives()){
                if (!mBases->contains(primitive->base())){
                    output << "[Error] Base " << primitive->base()->getName() << " not included in project! Required by primitive " <<
                              primitive->name() << " used by arrow from " << arrow->from()->position()->x() << "x" <<
                              arrow->from()->position()->y() << " to " << arrow->to()->position()->x() << "x" <<
                              arrow->to()->position()->y() << ".";
                    fail = true;
                } else {
                    if (!primitive->base()->primitives()->contains(primitive)){
                        output << "[Error] Base " << primitive->base()->getName() << " does not contain the primitive " <<
                                  primitive->name();
                        fail = true;
                    }
                }
            }
        }

    }

}
