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
    mainwindow.cpp \
    editor.cpp \
    NetGraph/arrow.cpp \
    NetGraph/node.cpp \
    canvas.cpp \
    addnode.cpp \
    arrowbutton.cpp \
    NetGraph/netgraph.cpp \
    addarrowexpressiondialog.cpp

HEADERS  += \
    mainwindow.h \
    editor.h \
    NetGraph/arrow.h \
    NetGraph/node.h \
    canvas.h \
    addnode.h \
    arrowbutton.h \
    NetGraph/netgraph.h \
    addarrowexpressiondialog.h

FORMS    += \
    mainwindow.ui \
    addnode.ui \
    addarrowexpressiondialog.ui

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PrimitivesLoader/release/ -lPrimitivesLoader
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PrimitivesLoader/debug/ -lPrimitivesLoader
#else:unix: LIBS += -L$$OUT_PWD/../PrimitivesLoader/ -lPrimitivesLoader

#INCLUDEPATH += $$PWD/../PrimitivesLoader
#DEPENDPATH += $$PWD/../PrimitivesLoader
