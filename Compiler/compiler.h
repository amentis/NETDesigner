#ifndef COMPILER_H
#define COMPILER_H

#include "compiler_global.h"

class QTextStream;

class COMPILERSHARED_EXPORT Compiler
{

public:
    Compiler();
    ~Compiler();
    bool checkForErrors(QTextStream& warnings, QTextStream& errors);
    bool translate(QTextStream& warnings, QTextStream& errors);
    bool compile(QTextStream& warnings, QTextStream& errors);
    bool build(QTextStream& warnings, QTextStream& errors);
};

#endif // COMPILER_H
