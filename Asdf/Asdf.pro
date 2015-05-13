#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T20:03:57
#
#-------------------------------------------------

QT       -= gui

TARGET = Asdf
TEMPLATE = lib
CONFIG += staticlib

SOURCES += asdf.cpp

HEADERS += asdf.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
