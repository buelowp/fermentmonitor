TEMPLATE = app
CONFIG += thread gui
TARGET = fermentmonitor
QT += network widgets

MOC_DIR = .moc
OBJECTS_DIR = .obj

SOURCES = main.cpp \
	BubbleMonitor.cpp \
	ConicalDisplay.cpp \
	FermenterGPIO.cpp \
	FermentMonitor.cpp \
	RestServer.cpp \
	StopWatch.cpp \
	TempMonitor.cpp \
	Thermostat.cpp \
	DHTMonitor.cpp
	
HEADERS = BubbleMonitor.h \
	ConicalDisplay.h \
	Fermenter.h \
	FermenterGPIO.h \
	FermentMonitor.h \
	RestServer.h \
	StopWatch.h \
	TempMonitor.h \
	Thermostat.h \
	DHTMonitor.h
	
