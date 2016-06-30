#-------------------------------------------------
#
# Project created by QtCreator 2013-11-15T21:02:33
# Last time edited 2014-07-27T17:14:22
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fraktal
TEMPLATE = app

static {
    CONFIG += static
    DEFINES += STATIC
    message("~~~ static build ~~~") # this is for information, that the static build is done
    mac: TARGET = $$join(TARGET,,,_static) #this adds an _static in the end, so you can seperate static build from non static build
    win32: TARGET = $$join(TARGET,,,s) #this adds an s in the end, so you can seperate static build from non static build
}
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

LIBS += -lOpenCL

win32: LIBS += "-LC:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/lib/Win32/" -lOpenCL

win32: INCLUDEPATH += "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/include"
win32: DEPENDPATH += "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v6.0/include"
