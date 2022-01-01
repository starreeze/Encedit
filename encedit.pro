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
        directory.cpp \
        mainwindow.cpp\
        encript.cpp\
        config.cpp\
        io.cpp

HEADERS  += mainwindow.h\
        directory.h \
        encript.h\
        config.h\
        io.h

FORMS    += mainwindow.ui \
    directory.ui

RESOURCES += \
    AppResources.qrc
