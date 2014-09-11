#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T21:51:08
#
#-------------------------------------------------

QT       += core gui sql

CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YogaPoints2
TEMPLATE = app


SOURCES += main.cpp\
    views/mainwindow.cpp \
    views/addpointsview.cpp \
    views/positionview.cpp \
    models/serie.cpp \
    models/yogapoint.cpp \
    models/position.cpp \
    managers/databasemanager.cpp \
    views/managepositionview.cpp \
    views/addpositionview.cpp \
    views/manageserieview.cpp

HEADERS  += views/mainwindow.h \
    views/addpointsview.h \
    views/positionview.h \
    models/serie.h \
    models/yogapoint.h \
    models/position.h \
    managers/databasemanager.h \
    views/managepositionview.h \
    views/addpositionview.h \
    views/manageserieview.h

RESOURCES += \
    resources.qrc
