TEMPLATE = app
DESTDIR = ../dist
INCLUDEPATH = ../src
LIBS += -L \
    ../dist \
    -l \
    geoqt
HEADERS = demowindow.h
SOURCES = demowindow.cpp \
    main.cpp
FORMS += demowindow.ui
RESOURCES += demo.qrc
