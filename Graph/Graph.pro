#-------------------------------------------------
#
# Project created by QtCreator 2015-05-16T16:59:52
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Graph
TEMPLATE = lib

DEFINES += GRAPH_LIBRARY

SOURCES += graph.cpp \
    arrow.cpp \
    node.cpp

HEADERS += graph.h\
        graph_global.h \
    arrow.h \
    node.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PrimitivesLoader/release/ -lPrimitivesLoader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PrimitivesLoader/debug/ -lPrimitivesLoader
else:unix: LIBS += -L$$OUT_PWD/../PrimitivesLoader/ -lPrimitivesLoader

INCLUDEPATH += $$PWD/../PrimitivesLoader
DEPENDPATH += $$PWD/../PrimitivesLoader
