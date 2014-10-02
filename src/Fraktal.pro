#-------------------------------------------------
#
# Project created by QtCreator 2013-11-15T21:02:33
# Last time edited 2014-07-27T17:14:22
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fraktal
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        renderarea.cpp \
        fraktal_manager.cpp \
        thread.cpp \
        fracfuncclass.cpp \
        fraktal_funcs.cpp \
        juliainputdialog.cpp \
        openclhandler.cpp

HEADERS  += mainwindow.h \
            renderarea.h \
            fraktal_manager.h \
            fraktal_List.h \
            fraktal_funcs.h \
            thread.h \
            fracfuncclass.h \
            juliainputdialog.h \
            openclhandler.h

FORMS    +=

TRANSLATIONS = fraktal_de.ts

win32: LIBS += "-LC:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/lib/Win32/" -lOpenCL

INCLUDEPATH += "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/include"
DEPENDPATH += "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/include"
