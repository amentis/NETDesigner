#-------------------------------------------------
#
# Project created by QtCreator 2015-05-16T16:48:51
#
#-------------------------------------------------

QT       -= gui

TARGET = Compiler
TEMPLATE = lib

DEFINES += COMPILER_LIBRARY

SOURCES += compiler.cpp

HEADERS += compiler.h\
        compiler_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
