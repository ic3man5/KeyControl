#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T01:54:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = key_control
TEMPLATE = app

QMAKE_CXXFLAGS += -std=gnu++14


SOURCES += main.cpp\
        widget.cpp \
    keycontrol.cpp

HEADERS  += widget.h \
    keycontrol.h

FORMS    += widget.ui

RESOURCES += \
    main.qrc
