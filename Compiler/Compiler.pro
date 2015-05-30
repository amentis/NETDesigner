#-------------------------------------------------
#
# Project created by QtCreator 2015-05-16T16:48:51
#
#-------------------------------------------------

CONFIG += c++11

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Graph/release/ -lGraph
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Graph/debug/ -lGraph
else:unix: LIBS += -L$$OUT_PWD/../Graph/ -lGraph

INCLUDEPATH += $$PWD/../Graph
DEPENDPATH += $$PWD/../Graph

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PrimitivesLoader/release/ -lPrimitivesLoader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PrimitivesLoader/debug/ -lPrimitivesLoader
else:unix: LIBS += -L$$OUT_PWD/../PrimitivesLoader/ -lPrimitivesLoader

INCLUDEPATH += $$PWD/../PrimitivesLoader
DEPENDPATH += $$PWD/../PrimitivesLoader

unix|win32: LIBS += -lclang

unix|win32: LIBS += -lLLVM-3.6
