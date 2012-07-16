QT += network
TARGET = geoqt
VERSION = 0.1.0
TEMPLATE = lib
DESTDIR = ../dist
SOURCES += mapwidget.cpp \
	tileprovider.cpp \
	abstractmarker.cpp \
	balloonmarker.cpp
HEADERS += mapwidget.h \
	tileprovider.h \
	mapsourceinterface.h \
	abstractmarker.h \
	balloonmarker.h

PRIVATE_HEADERS += private/mapwidget_p.h \
	private/tileprovider_p.h \
	private/abstractmarker_p.h \
	private/balloonmarker_p.h

HEADERS += $${PRIVATE_HEADERS}
