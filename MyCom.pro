#-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T14:27:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyCom
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qextserialport_win.cpp \
    qextserialport.cpp

HEADERS  += widget.h \
    qextserialport_global.h \
    qextserialport.h \
    qextserialport_p.h

FORMS    += widget.ui
