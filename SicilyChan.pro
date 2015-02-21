#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T10:49:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SicilyChan
TEMPLATE = app
CONFIG += static

SOURCES += main.cpp\
    Defines.cpp \
    trans.cpp \
    mainwindow.cpp

HEADERS  += \
    Defines.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui

win32: LIBS += -L$$PWD/../../App/Python27/libs/ -lpython27

INCLUDEPATH += $$PWD/../../App/Python27/include
DEPENDPATH += $$PWD/../../App/Python27/include
