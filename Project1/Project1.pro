#-------------------------------------------------
#
# Project created by QtCreator 2012-07-16T11:54:48
#
#-------------------------------------------------


TEMPLATE = app

DESTDIR = ../dist
INCLUDEPATH = ../src
LIBS += -L \
    ../dist \
    -l \
    geoqt


SOURCES = main.cpp\
        projectwidget.cpp

HEADERS  = projectwidget.h

FORMS    += projectwidget.ui
