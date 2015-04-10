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

FermentMonitor::FermentMonitor(QFrame *parent) : QFrame(parent) {
	temps = new TempMonitor(this);
	restHandler = new RestServer(80);
	thermostat = new Thermostat();
	leftConical = new ConicalDisplay(this);
	rightConical = new ConicalDisplay(this);

	leftConical->setGeometry(0, 0, 800, 120);
	rightConical->setGeometry(0, 120, 800, 120);

	lbBoxTemp = new QLabel(this);
	lbBoxTemp->setNum(0.0);
	lbLeftTime = new QLabel(this);
	lbRightTime = new QLabel(this);

	layout = new QHBoxLayout(this);
	layout->setGeometry(0, 360, 800, 120);
	layout->addWidget(lbBoxTemp);
	layout->addWidget(lbLeftTime);
	layout->addWidget(lbRightTime);

	connect(thermostat, SIGNAL(thermostatAlarm(enum ThermAlarm)), this, SLOT(thermostatAlarm(enum ThermAlarm)));
	connect(this, SIGNAL(updateLeftBPM(int)), leftConical, SLOT(updateBPM(int)));
	connect(this, SIGNAL(updateLeftTemp(double)), leftConical, SLOT(updateTemp(double)));
	connect(this, SIGNAL(leftConicalError()), leftConical, SLOT(error()));
	connect(this, SIGNAL(updateRightBPM(int)), rightConical, SLOT(updateBPM(int)));
	connect(this, SIGNAL(updateRightTemp(double)), rightConical, SLOT(updateTemp(double)));
	connect(this, SIGNAL(rightConicalError()), rightConical, SLOT(error()));
	connect(leftConical, SIGNAL(updateRuntime(QString)), lbLeftTime, SLOT(setText(QString)));
	connect(rightConical, SIGNAL(updateRuntime(QString)), lbRightTime, SLOT(setText(QString)));
}

FermentMonitor::~FermentMonitor()
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
		 lbBoxTemp->setNum(temp);
	 }
	 if (name == "leftconical") {
		 emit updateLeftTemp(temp);
		 thermostat->currFermOneTemp(temp);
	 }
	 if (name == "rightconical") {
		 emit updateRightTemp(temp);
		 thermostat->currFermTwoTemp(temp);
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
	QFile configFile(xmlFile + ".config/fermentmon.xml");

	if (!configFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
		return false;
	}
	xml.setDevice(&configFile);

	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isStartElement()) {
			QString tag = xml.name().toString();
			if (tag == "gpio") {
				QXmlStreamAttributes attributes = xml.attributes();
				QString attribute = attributes.value("name");
				if (attribute == "heater") {
					thermostat->addHeaterGPIO(attributes.value("path").toString());
				}
				if (attribute == "cooler") {
					thermostat->addCoolerGPIO(attributes.value("path").toString());
				}
			}
			if (tag == "thermometer") {
				QXmlStreamAttributes attributes = xml.attributes();
				QString name = attributes.value("name");
				QString path = attributes.value("path");
				temps->addDevice(name, path);
			}
			if (tag == "holdtemp") {
				QXmlStreamAttributes attributes = xml.attributes();
				thermostat->setFermenterTemp(attributes.value("temp").toDouble());
			}
			if (tag == "counter") {
				QXmlStreamAttributes attributes = xml.attributes();
				BubbleMonitor *bm = new BubbleMonitor(attributes.value("path"), attributes.value("name"));
				if (bm->isOpen()) {
					bubbleCounters.insert(attributes.value("name"), bm);
					connect(bm, SIGNAL(bubbleCount(QString, int)), this, SLOT(bubbleCount(QString, int)));
					connect(bm, SIGNAL(fermentationComplete(QString)), this, SLOT(fementationComplete(QString)));
				}
			}
			if (tag == "conical") {

			}
		}
	}
}
