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
	leftConical = new ConicalDisplay(this);
	rightConical = new ConicalDisplay(this);

	leftConical->setGeometry(5, 5, 790, 170);
	rightConical->setGeometry(5, 180, 790, 170);

	m_leftGravity = new Gravity(this);
	m_rightGravity = new Gravity(this);

	lbExternalTemp = new QLabel(this);
	lbExternalTemp->setAlignment(Qt::AlignCenter);
	lbExternalTemp->setText(QString("<font style='font-size:20pt;'>Air Temp</font><br><font style='font-size:52pt;color:green;'>%1</font> <font style='font-size:20pt'>%2F</font>").arg((double)75.1).arg(QChar(0xB0)));
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
	m_envHumidity = new QLabel(this);
	m_envHumidity->setAlignment(Qt::AlignCenter);
	m_envHumidity->setText(QString("<font style='font-size:20pt;'>Humidity</font><br><font style='font-size:52pt;color:green;'>%1</font> <font style='font-size:20pt'>%2F</font>").arg((double)75.1).arg(QChar(0xB0)));
	m_envHumidity->setStyleSheet(".QLabel{border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");
	m_envHumidity->setGeometry(5, 360, 195, 115);

	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);

	connect(this, SIGNAL(updateLeftBPM(int)), leftConical, SLOT(updateBPM(int)));
	connect(this, SIGNAL(leftConicalError()), leftConical, SLOT(error()));
	connect(this, SIGNAL(updateRightBPM(int)), rightConical, SLOT(updateBPM(int)));
	connect(this, SIGNAL(rightConicalError()), rightConical, SLOT(error()));
	connect(m_leftGravity, SIGNAL(updateGravity(double)), leftConical, SLOT(updateGravity(double)));
	connect(m_rightGravity, SIGNAL(updateGravity(double)), rightConical, SLOT(updateGravity(double)));
	connect(temps, SIGNAL(probeUpdate(QString, double)), this, SLOT(updateTemps(QString, double)));
	connect(leftConical, SIGNAL(updateRuntime(QString)), lbLeftTime, SLOT(setText(QString)));
	connect(rightConical, SIGNAL(updateRuntime(QString)), lbRightTime, SLOT(setText(QString)));
}

FermentMonitor::~FermentMonitor()
{
}

void FermentMonitor::updateTemps(QString n, double t)
{
}

void FermentMonitor::bubbleCount(QString name, int count)
{
	BubbleMonitor *bm = bubbleCounters.value(name);
	restHandler->setBubbleCount(name, count);
	restHandler->setBubblesPerMin(name, bm->bubblesPerMinute());
	if (name == "left") {
		emit updateLeftBPM(bm->bubblesPerMinute());
	}
	if (name == "right") {
		emit updateRightBPM(bm->bubblesPerMinute());
	}
}

void FermentMonitor::fermentationComplete(QString name)
{
	restHandler->setFermentState(name, true);
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
			}
			if (tag == "thermometer") {
				QXmlStreamAttributes attributes = xml.attributes();
				QString name = attributes.value("name").toString();
				QString path = attributes.value("path").toString();
				temps->addDevice(name, path);
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
				if (attributes.value("which") == "left") {
					leftConical->setName(attributes.value("name").toString());
				}
				if (attributes.value("which") == "right") {
					rightConical->setName(attributes.value("name").toString());
				}
			}
			if (tag == "gravity") {
				QXmlStreamAttributes attributes = xml.attributes();
				if (attributes.value("name") == "left") {
					m_leftGravity->setName(attributes.value("name").toString());
					m_leftGravity->setDevice(attributes.value("device").toInt());
				}
				if (attributes.value("name") == "right") {
					m_rightGravity->setName(attributes.value("name").toString());
					m_rightGravity->setDevice(attributes.value("device").toInt());
				}
			}
		}
	}

	temps->start();
	return true;
}
