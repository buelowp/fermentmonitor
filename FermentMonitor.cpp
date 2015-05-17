/*
 * FermentMonitor.cpp
 *
 *  This file is part of FermentMonitor.
 *
 *  FermentMonitor is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  FermentMonitor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with FermentMonitor.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: Feb 2, 2015
 *      Author: pete
 */

#include "FermentMonitor.h"

FermentMonitor::FermentMonitor(QWidget *parent, Qt::WindowFlags f) : QFrame(parent, f) {
	temps = new TempMonitor();
	restHandler = new RestServer(80);
	thermostat = new Thermostat();
	leftConical = new ConicalDisplay(this);
	rightConical = new ConicalDisplay(this);

	leftConical->setGeometry(5, 5, 790, 170);
	rightConical->setGeometry(5, 180, 790, 170);

	tDHTTimer = new QTimer();

	lbBoxTemp = new QLabel(this);
	lbBoxTemp->setAlignment(Qt::AlignCenter);
	lbBoxTemp->setText(QString("<font style='font-size:20pt;'>Internal</font><br><font style='font-size:52pt;color:green;'>%1</font> <font style='font-size:20pt'>%2F</font>").arg((double)75.1).arg(QChar(0xB0)));
	lbBoxTemp->setStyleSheet(".QLabel{border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");
	lbBoxTemp->setGeometry(5, 360, 195, 115);
	lbExternalTemp = new QLabel(this);
	lbExternalTemp->setAlignment(Qt::AlignCenter);
	lbExternalTemp->setText(QString("<font style='font-size:20pt;'>External</font><br><font style='font-size:52pt;color:green;'>%1</font> <font style='font-size:20pt'>%2F</font>").arg((double)75.1).arg(QChar(0xB0)));
	lbExternalTemp->setStyleSheet(".QLabel{border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");
	lbExternalTemp->setGeometry(205, 360, 195, 115);
	lbLeftTime = new QLabel(this);
	lbLeftTime->setAlignment(Qt::AlignCenter);
	lbLeftTime->setStyleSheet(".QLabel{color: black; border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");
	lbLeftTime->setGeometry(405, 360, 195, 115);
	lbRightTime = new QLabel(this);
	lbRightTime->setAlignment(Qt::AlignCenter);
	lbRightTime->setStyleSheet(".QLabel{color: black; border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");
	lbRightTime->setGeometry(605, 360, 190, 115);

	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);

	connect(thermostat, SIGNAL(thermostatAlarm(enum ThermAlarms)), this, SLOT(thermostatAlarm(enum ThermAlarms)));
	connect(this, SIGNAL(updateLeftBPM(int)), leftConical, SLOT(updateBPM(int)));
	connect(this, SIGNAL(updateLeftTemp(double)), leftConical, SLOT(updateTemp(double)));
	connect(this, SIGNAL(leftConicalError()), leftConical, SLOT(error()));
	connect(this, SIGNAL(updateRightBPM(int)), rightConical, SLOT(updateBPM(int)));
	connect(this, SIGNAL(updateRightTemp(double)), rightConical, SLOT(updateTemp(double)));
	connect(this, SIGNAL(rightConicalError()), rightConical, SLOT(error()));
	connect(leftConical, SIGNAL(updateRuntime(QString)), lbLeftTime, SLOT(setText(QString)));
	connect(rightConical, SIGNAL(updateRuntime(QString)), lbRightTime, SLOT(setText(QString)));
	connect(temps, SIGNAL(probeUpdate(QString, double)), this, SLOT(tempChange(QString, double)));
	connect(tDHTTimer, SIGNAL(timeout()), this, SLOT(getDHTValues()));

	tDHTTimer->start(1000);
}

FermentMonitor::~FermentMonitor()
{
}

void FermentMonitor::getDHTValues()
{
	QString s1;
	QString s2;
	if (dhtMon.isValid()) {
		s1 = QString().setNum(dhtMon.getTemperature(), 'f', 1);
		lbBoxTemp->setText(QString("<font style='font-size:20pt;'>Temperature</font><br><font style='font-size:48pt; color:black'>%1</font> <font style='font-size:20pt'>F</font>").arg(s1));
		s2 = QString().setNum(dhtMon.getHumidity(), 'f', 1);
		lbExternalTemp->setText(QString("<font style='font-size:20pt;'>Humidity</font><br><font style='font-size:48pt; color:black'>%1</font> <font style='font-size:20pt'>%</font>").arg(s2));
	}
	else {
		qDebug() << "dhtMon is reporting invalid";
	}
}

void FermentMonitor::thermostatAlarm(enum ThermAlarms)
{

}

void FermentMonitor::bubbleCount(QString name, int count)
{
	BubbleMonitor *bm = bubbleCounters.value(name);
	restHandler->setBubbleCount(name, count);
	restHandler->setBubblesPerMin(name, bm->bubblesPerMinute());
	if (name == "leftconical") {
		emit updateLeftBPM(bm->bubblesPerMinute());
	}
	if (name == "rightconical") {
		emit updateRightBPM(bm->bubblesPerMinute());
	}
}

void FermentMonitor::tempChange(QString name, double temp)
{
	 restHandler->setTemp(name, temp);
	 if (name == "chamber") {
		 thermostat->currBoxTemp(temp);
		 if (temp < 50) {
			 lbBoxTemp->setText(QString("%1 %2F").arg(temp).arg(QChar(0xB0)));
			 lbBoxTemp->setStyleSheet(".QLabel{font: 36pt; color: blue; border-radius: 5px; border-style: solid; border-width: 1px;}");
		 }
		 else if (temp > 80) {
			 lbBoxTemp->setText(QString("%1 %2F").arg(temp).arg(QChar(0xB0)));
			 lbBoxTemp->setStyleSheet(".QLabel{font: 36pt; color: red; border-radius: 5px; border-style: solid; border-width: 1px;}");
		 }
		 else {
			 lbBoxTemp->setText(QString("%1 %2F").arg(temp).arg(QChar(0xB0)));
			 lbBoxTemp->setStyleSheet(".QLabel{font: 36pt; color: green; border-radius: 5px; border-style: solid; border-width: 1px;}");
		 }
	 }
	 if (name == "leftconical") {
		 emit updateLeftTemp(temp);
		 thermostat->currFermOneTemp(temp);
		 leftConical->updateTemp(temp);
	 }
	 if (name == "rightconical") {
		 emit updateRightTemp(temp);
		 thermostat->currFermTwoTemp(temp);
		 rightConical->updateTemp(temp);
	 }
}

void FermentMonitor::fermentationComplete(QString name)
{
	restHandler->setFermentState(name, true);
	thermostat->shutdown();
}

bool FermentMonitor::init()
{
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	QXmlStreamReader xml;
	QString xmlFile = env.value("HOME");
	QFile configFile(xmlFile + "/.config/fermentmonitor.xml");

	if (!configFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
		qDebug() << "Error opening " << configFile.fileName() << " with error " << configFile.error();
		return false;
	}
	xml.setDevice(&configFile);

	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isStartElement()) {
			QString tag = xml.name().toString();
			if (tag == "gpio") {
				QXmlStreamAttributes attributes = xml.attributes();
				QString attribute = attributes.value("name").toString();
				if (attribute == "heater") {
					thermostat->addHeaterGPIO(attributes.value("path").toString());
				}
				if (attribute == "cooler") {
					thermostat->addCoolerGPIO(attributes.value("path").toString());
				}
			}
			if (tag == "thermometer") {
				QXmlStreamAttributes attributes = xml.attributes();
				QString name = attributes.value("name").toString();
				QString path = attributes.value("path").toString();
				QString cal = attributes.value("calibration").toString();
				temps->addDevice(name, path);
				if (cal.size()) {
					temps->setCalibration(cal.toDouble());
				}
			}
			if (tag == "holdtemp") {
				QXmlStreamAttributes attributes = xml.attributes();
				thermostat->currBoxTemp(attributes.value("temp").toString().toDouble());
				rightConical->setHoldTemp(attributes.value("temp").toString().toDouble());
				leftConical->setHoldTemp(attributes.value("temp").toString().toDouble());
			}
			if (tag == "counter") {
				QXmlStreamAttributes attributes = xml.attributes();
				BubbleMonitor *bm = new BubbleMonitor(attributes.value("path").toString(), attributes.value("name").toString());
				if (bm->isOpen()) {
					bubbleCounters.insert(attributes.value("name").toString(), bm);
					connect(bm, SIGNAL(bubbleCount(QString, int)), this, SLOT(bubbleCount(QString, int)));
					connect(bm, SIGNAL(fermentationComplete(QString)), this, SLOT(fementationComplete(QString)));
				}
			}
			if (tag == "conical") {
				QXmlStreamAttributes attributes = xml.attributes();
				if (attributes.value("which") == "leftconical") {
					leftConical->setName(attributes.value("name").toString());
				}
				if (attributes.value("which") == "rightconical") {
					rightConical->setName(attributes.value("name").toString());
				}
			}
		}
	}

	temps->start();
	dhtMon.init();
	dhtMon.setCalibration(3);
	return true;
}
