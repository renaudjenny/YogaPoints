#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T21:51:08
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YogaPoints2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addpointsview.cpp \
    positionview.cpp \
    serie.cpp \
    yogapoint.cpp \
    position.cpp \
    databasemanager.cpp

HEADERS  += mainwindow.h \
    addpointsview.h \
    positionview.h \
    serie.h \
    yogapoint.h \
    position.h \
    databasemanager.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    positions.json
