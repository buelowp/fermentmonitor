TEMPLATE = app
CONFIG += thread gui
TARGET = fermentmonitor
QT += network widgets

SOURCES = main.cpp \
	BubbleMonitor.cpp \
	ConicalDisplay.cpp \
	FermenterGPIO.cpp \
	FermentMonitor.cpp \
	RestServer.cpp \
	StopWatch.cpp \
	TempMonitor.cpp \
	Thermostat.cpp
	
HEADERS = BubbleMonitor.h \
	ConicalDisplay.h \
	Fermenter.h \
	FermenterGPIO.h \
	FermentMonitor.h \
	RestServer.h \
	StopWatch.h \
	TempMonitor.h \
	Thermostat.h
	
