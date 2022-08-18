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
        indexmodel.cpp \
        mainwindow.cpp\
        encript.cpp\
        config.cpp

HEADERS  += mainwindow.h\
        args.h \
        encript.h\
        config.h \
        indexmodel.h

FORMS    += mainwindow.ui

RESOURCES += \
    AppResources.qrc
