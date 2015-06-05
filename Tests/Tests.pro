#-------------------------------------------------
#
# Project created by QtCreator 2015-06-05T16:12:10
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = Tests

CONFIG += testcase c++11

SOURCES += \
    testcompiler.cpp \
    testdesigner.cpp \
    testgraph.cpp \
    testprimitivesloader.cpp \
    main.cpp \
#designer
    $$PWD/../Designer/mainwindow.cpp \
    $$OUT_PWD/../Designer/moc_mainwindow.cpp \
    $$PWD/../Designer/addnode.cpp \
    $$OUT_PWD/../Designer/moc_addnode.cpp \
    $$PWD/../Designer/editor.cpp \
    $$OUT_PWD/../Designer/moc_editor.cpp \
    $$PWD/../Designer/addarrowexpressiondialog.cpp \
    $$OUT_PWD/../Designer/moc_addarrowexpressiondialog.cpp \
    $$PWD/../Designer/arrowbutton.cpp \
    $$OUT_PWD/../Designer/moc_arrowbutton.cpp \
    $$PWD/../Designer/browseprimitivesdialog.cpp \
    $$OUT_PWD/../Designer/moc_browseprimitivesdialog.cpp \
    $$PWD/../Designer/canvas.cpp \
    $$OUT_PWD/../Designer/moc_canvas.cpp \
    $$PWD/../Designer/editarrowdialog.cpp \
    $$OUT_PWD/../Designer/moc_editarrowdialog.cpp \
    $$PWD/../Designer/programoptionsdialog.cpp \
    $$OUT_PWD/../Designer/moc_programoptionsdialog.cpp

HEADERS += \
    testcompiler.h \
    testdesigner.h \
    testgraph.h \
    testprimitivesloader.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Compiler/release/ -lCompiler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Compiler/debug/ -lCompiler
else:unix: LIBS += -L$$OUT_PWD/../Compiler/ -lCompiler

INCLUDEPATH += $$PWD/../Compiler
DEPENDPATH += $$PWD/../Compiler

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

INCLUDEPATH +=$$PWD/../Designer
INCLUDEPATH +=$$OUT_PWD/../Designer
