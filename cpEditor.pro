#-------------------------------------------------
#
# Project created by QtCreator 2017-01-15T17:48:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cpEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    highlighter.cpp \
    codeeditor.cpp \
    customtab.cpp \
    openfile.cpp \
    debuggerdebugging.cpp \
    showfunctionsdialog.cpp \
    showfunction.cpp \
    codeviewer.cpp \
    showiterationsdialog.cpp

HEADERS  += mainwindow.h \
    highlighter.h \
    codeeditor.h \
    customtab.h \
    openfile.h \
    debuggerdebugging.h \
    structures.h \
    showfunctionsdialog.h \
    showfunction.h \
    codeviewer.h \
    showiterationsdialog.h

FORMS    += mainwindow.ui \
    openfile.ui \
    debuggerdebugging.ui \
    showfunctionsdialog.ui \
    showfunction.ui \
    showiterationsdialog.ui

RESOURCES += \
    completelist.qrc

DISTFILES +=
