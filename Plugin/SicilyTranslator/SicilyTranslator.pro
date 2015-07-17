#-------------------------------------------------
#
# Project created by QtCreator 2015-07-17T13:35:06
#
#-------------------------------------------------

QT       += core
QT       += network
QMAKE_CXXFLAGS += -std=c++11

#QT       += gui
QT       += widgets


TARGET = SicilyTranslator
#CONFIG   += console
#CONFIG += release

CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Trans.cpp

HEADERS += \
    Trans.h
