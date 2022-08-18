#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T18:28:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = encedit
TEMPLATE = app


SOURCES += main.cpp\
        crypto.cpp \
        indexmodel.cpp \
        io.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h\
        args.h \
        crypto.h \
        indexmodel.h \
        io.h

FORMS    += mainwindow.ui

RESOURCES += \
    AppResources.qrc
