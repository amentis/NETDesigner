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

unix {
    target.path = /usr/bin
    INSTALLS += target
}

SOURCES += main.cpp \
    mainwindow.cpp \
    editor.cpp \
    canvas.cpp \
    addnode.cpp \
    arrowbutton.cpp \
    addarrowexpressiondialog.cpp \
    browseprimitivesdialog.cpp \
    programoptionsdialog.cpp \
    editarrowdialog.cpp

HEADERS  += \
    mainwindow.h \
    editor.h \
    canvas.h \
    addnode.h \
    arrowbutton.h \
    addarrowexpressiondialog.h \
    browseprimitivesdialog.h \
    programoptionsdialog.h \
    editarrowdialog.h

FORMS    += \
    mainwindow.ui \
    addnode.ui \
    addarrowexpressiondialog.ui \
    browseprimitivesdialog.ui \
    programoptionsdialog.ui \
    editarrowdialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PrimitivesLoader/release/ -lPrimitivesLoader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PrimitivesLoader/debug/ -lPrimitivesLoader
else:unix: LIBS += -L$$OUT_PWD/../PrimitivesLoader/ -lPrimitivesLoader

INCLUDEPATH += $$PWD/../PrimitivesLoader
DEPENDPATH += $$PWD/../PrimitivesLoader

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Graph/release/ -lGraph
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Graph/debug/ -lGraph
else:unix: LIBS += -L$$OUT_PWD/../Graph/ -lGraph

INCLUDEPATH += $$PWD/../Graph
DEPENDPATH += $$PWD/../Graph

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Compiler/release/ -lCompiler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Compiler/debug/ -lCompiler
else:unix: LIBS += -L$$OUT_PWD/../Compiler/ -lCompiler

INCLUDEPATH += $$PWD/../Compiler
DEPENDPATH += $$PWD/../Compiler
