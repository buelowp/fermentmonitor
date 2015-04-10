/*
 * FermenterGPIO.cpp
 *
 *  Created on: Apr 7, 2015
 *      Author: pete
 */

#include "FermenterGPIO.h"

FermenterGPIO::FermenterGPIO(QObject *parent) : QObject(parent) {

}

FermenterGPIO::~FermenterGPIO() {
	// TODO Auto-generated destructor stub
}

bool FermenterGPIO::setGPIO(QString gpio)
{
	QDir dir(gpio);
	QStringList entries;

	if (dir.exists()) {
		entries = dir.entryList();
		for (int i = 0; i < entries.size(); ++i) {
			if (entries.at(i) == "value") {
				qfValue = new QFile(gpio + "/value");
			}
			if (entries.at(i) == "direction") {
				qfDirection = new QFile(gpio + "/direction");
			}
		}
	}
	else {
		return false;
	}

	if (!qfValue && !qfDirection)
		return false;

	qfswGPIO = new QFileSystemWatcher();
	qfswGPIO->addPath(qfValue->fileName());
	connect(qfswGPIO, SIGNAL(fileChanged(QString)), this, SLOT(changed(QString)));

	return true;
}

void FermenterGPIO::setDirection(QByteArray dir)
{
	if (dir != "in" || dir != "out") {
		return;
	}
	qfDirection->write(dir);
}

void FermenterGPIO::changed(QString)
{
	QByteArray ba = qfValue->readAll();
	emit valueChanged(ba);
}

bool FermenterGPIO::checkValue(QString val)
{
	QByteArray ba = qfValue->readAll();
	if (ba == val)
		return true;

	return false;
}

QByteArray FermenterGPIO::getValue()
{
	return qfValue->readAll();
}

void FermenterGPIO::setValue(QByteArray ba)
{
	qfValue->write(ba.constData());
}
