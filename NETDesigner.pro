#-------------------------------------------------
#
# Project created by QtCreator 2015-03-15T20:02:39
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NETDesigner
TEMPLATE = app


SOURCES += main.cpp \
    Designer/mainwindow.cpp \
    Designer/editor.cpp \
    PrimitivesLoader/base.cpp \
    PrimitivesLoader/primitive.cpp \
    NetGraph/arrow.cpp \
    NetGraph/node.cpp \
    NetGraph/ordinarynode.cpp \
    NetGraph/proximitynode.cpp \
    NetGraph/casenode.cpp \
    Designer/canvas.cpp \
    Designer/addnode.cpp \
    NetGraph/startnode.cpp \
    NetGraph/endnode.cpp \
    Designer/arrowbutton.cpp \
    NetGraph/netgraph.cpp

HEADERS  += \
    Designer/mainwindow.h \
    Designer/editor.h \
    PrimitivesLoader/base.h \
    PrimitivesLoader/primitive.h \
    NetGraph/arrow.h \
    NetGraph/node.h \
    NetGraph/ordinarynode.h \
    NetGraph/proximitynode.h \
    NetGraph/casenode.h \
    Designer/canvas.h \
    NetGraph/startnode.h \
    NetGraph/endnode.h \
    Designer/addnode.h \
    Designer/arrowbutton.h \
    NetGraph/netgraph.h

FORMS    += \
    Designer/mainwindow.ui \
    Designer/addnode.ui
