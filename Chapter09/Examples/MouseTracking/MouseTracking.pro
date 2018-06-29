QT += core widgets

CONFIG += c++14

TARGET = MouseTracking
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += rxqt-master/include


SOURCES += \
    main.cpp

HEADERS += \
    gravity_qlabel.h
