######################################################################
# Automatically generated by qmake (3.1) Tue Jun 16 16:41:46 2020
######################################################################

QT += quickcontrols2
QT += testlib
QT += quick
QT += core

TEMPLATE = app
TARGET = test
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += TestInputProcessor.h ../classes/*.h
SOURCES += mainTest.cpp TestInputProcessor.cpp ../classes/*.cpp
