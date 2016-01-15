#-------------------------------------------------
#
# Project created by QtCreator 2016-01-13T20:18:02
#
#-------------------------------------------------

QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = roads-graph
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    osmdialog.cpp

HEADERS  += mainwindow.h \
    osmdialog.h

FORMS    += mainwindow.ui \
    osmdialog.ui

DISTFILES += \
    index.html

RESOURCES += \
    res.qrc

CONFIG += c++11

LIBS += -lboost_system -lboost_graph
