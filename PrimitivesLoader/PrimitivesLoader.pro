#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T20:51:14
#
#-------------------------------------------------

QT       -= gui

TARGET = PrimitivesLoader
TEMPLATE = lib

CONFIG += c++11

DEFINES += PRIMITIVESLOADER_LIBRARY

SOURCES += primitivesloader.cpp \
        primitive.cpp \
        base.cpp

HEADERS += primitivesloader.h\
        primitivesloader_global.h\
        primitive.h \
        base.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
