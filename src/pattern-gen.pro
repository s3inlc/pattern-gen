#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T19:46:26
# Copyright 2014-2016 by Sein Coray
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pattern-Gen
TEMPLATE = app

#QMAKE_LIBS += -Bstatic -static
#QMAKE_LIBS += -static-libgcc -static-libstdc++

SOURCES += main.cpp\
        start.cpp \
    options.cpp \
    util.cpp \
    calculation.cpp \
    charsets.cpp

HEADERS  += start.h \
    options.h \
    util.h \
    calculation.h \
    charsets.h

FORMS    += start.ui
