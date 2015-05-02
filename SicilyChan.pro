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

SOURCES +=\
    Defines.cpp \
    Sicily.cpp \
    main.cpp \
    Trans.cpp

HEADERS  += \
    Sicily.h \
    Defines.h \
    Trans.h

FORMS    += \
    Sicily.ui

win32: LIBS += -L$$PWD/../../App/Python27/libs/ -lpython27

win32: INCLUDEPATH += $$PWD/../../App/Python27/include
win32: DEPENDPATH += $$PWD/../../App/Python27/include

linux: INCLUDEPATH += -I /usr/include/python2.7/
linux: LIBS += -L /usr/lib/python2.7/ -lpython2.7

RC_FILE += ico.rc

OTHER_FILES += \
    sicily.ico \
    ico.rc
