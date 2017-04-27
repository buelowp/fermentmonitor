TEMPLATE = app
CONFIG += thread gui debug
TARGET = fermentmonitor
QT += gui network widgets

LIBS = -lvl6180_pi

SOURCES = main.cpp \
	BubbleMonitor.cpp \
	ConicalDisplay.cpp \
	FermentMonitor.cpp \
	RestServer.cpp \
	TempMonitor.cpp \
	DisplayPower.cpp \
    Gravity.cpp

HEADERS = BubbleMonitor.h \
	ConicalDisplay.h \
	Fermenter.h \
	FermentMonitor.h \
	RestServer.h \
	TempMonitor.h \
	DisplayPower.h \
    Gravity.h

