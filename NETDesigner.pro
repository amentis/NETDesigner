#-------------------------------------------------
#
# Project created by QtCreator 2015-03-15T20:02:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NETDesigner
TEMPLATE = app


SOURCES += main.cpp \
    Designer/mainwindow.cpp \
    Designer/editor.cpp \
    Designer/renderarea.cpp

HEADERS  += \
    Designer/mainwindow.h \
    Designer/editor.h \
    Designer/renderarea.h

FORMS    += \
    Designer/mainwindow.ui
