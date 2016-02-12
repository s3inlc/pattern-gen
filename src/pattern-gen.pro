#
# $Id: Pattern-Gen-v2.pro 11 2015-02-18 17:51:13Z sein $
#

#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T19:46:26
# Copyright 2014-2015 by Sein Coray
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pattern-Gen-v2
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
