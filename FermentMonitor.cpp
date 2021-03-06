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
	leftConical = new ConicalDisplay();
	rightConical = new ConicalDisplay();

	m_humidity = 0;
	m_externalTemp = 0;

	m_leftGravity = new Gravity(this);
	m_rightGravity = new Gravity(this);

	lbExternalTemp = new QLabel(this);
	lbExternalTemp->setAlignment(Qt::AlignCenter);
	lbExternalTemp->setText(QString("<font style='font-family: \"Roboto\"; font-size:20pt;'>Air Temp</font><br><font style='font-family: \"Roboto\"; font-size:52pt;'>%1</font> <font style='font-family: \"Roboto\"; font-size:20pt'>%2F</font>").arg(m_externalTemp).arg(QChar(0xB0)));
	lbExternalTemp->setStyleSheet(".QLabel{border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");
	lbLeftTime = new QLabel(this);
	lbLeftTime->setAlignment(Qt::AlignCenter);
	lbLeftTime->setStyleSheet(".QLabel{font-family: \"Roboto\"; color: black; border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");
	lbRightTime = new QLabel(this);
	lbRightTime->setAlignment(Qt::AlignCenter);
	lbRightTime->setStyleSheet(".QLabel{font-family: \"Roboto\"; color: black; border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");
	m_envHumidity = new QLabel(this);
	m_envHumidity->setAlignment(Qt::AlignCenter);
	m_envHumidity->setText(QString("<font style='font-family: \"Roboto\"; font-size:20pt;'>Humidity</font><br><font style='font-family: \"Roboto\"; font-size:52pt;'>%1</font> <font style='font-family: \"Roboto\"; font-size:20pt'>%</font>").arg(m_humidity));
	m_envHumidity->setStyleSheet(".QLabel{border-radius: 5px; border-style: solid; border-width: 1px; background-color: #ededed;}");

	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);

	connect(this, SIGNAL(leftConicalError()), leftConical, SLOT(error()));
	connect(this, SIGNAL(rightConicalError()), rightConical, SLOT(error()));
	connect(m_leftGravity, SIGNAL(gravityUpdated(double)), leftConical, SLOT(updateGravity(double)));
	connect(m_rightGravity, SIGNAL(gravityUpdated(double)), rightConical, SLOT(updateGravity(double)));
	connect(temps, SIGNAL(probeUpdate(QString, double)), this, SLOT(updateTemps(QString, double)));
	connect(leftConical, SIGNAL(updateRuntime(QString)), lbLeftTime, SLOT(setText(QString)));
	connect(rightConical, SIGNAL(updateRuntime(QString)), lbRightTime, SLOT(setText(QString)));
    connect(leftConical, SIGNAL(startRunning(bool)), m_leftGravity, SLOT(startRunning(bool)));
    connect(rightConical, SIGNAL(startRunning(bool)), m_rightGravity, SLOT(startRunning(bool)));
    connect(leftConical, SIGNAL(startRunning(bool)), this, SLOT(leftRunning(bool)));
    connect(rightConical, SIGNAL(startRunning(bool)), this, SLOT(rightRunning(bool)));
}

FermentMonitor::~FermentMonitor()
{
}

void FermentMonitor::leftRunning(bool r)
{
	if (!r) {
		lbLeftTime->setText("");
	}
}

void FermentMonitor::rightRunning(bool r)
{
	if (!r) {
		lbRightTime->setText("");
	}
}

void FermentMonitor::checkRuntime(QString r)
{
	qWarning() << __PRETTY_FUNCTION__ << r;
}

void FermentMonitor::showEvent(QShowEvent* e)
{
	leftConical->setGeometry(0, 0, width(), 170);
	rightConical->setGeometry(0, 180, width(), 170);
	lbExternalTemp->setGeometry(205, 360, 195, 115);
	lbLeftTime->setGeometry(405, 360, 195, 115);
	lbRightTime->setGeometry(605, 360, 190, 115);
	m_envHumidity->setGeometry(5, 360, 195, 115);
	leftConical->setParent(this);
	leftConical->show();
	rightConical->setParent(this);
	rightConical->show();
}

void FermentMonitor::updateTemps(QString n, double t)
{
    if (n == "left") {
        leftConical->updateTemp(t);
    }
    if (n == "right") {
        rightConical->updateTemp(t);
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
