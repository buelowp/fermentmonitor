/*
 * Thermostat.cpp
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
 *  Created on: April 3, 2015
 *      Author: Peter Buelow
 */

#include "Thermostat.h"

Thermostat::Thermostat(QObject *parent) : QObject(parent)
{
	bCoolerIsRunning = false;
	bHeaterIsRunning = false;
	bHeaterEnabled = false;
	bCoolerEnabled = false;
	bCoolerTimeout = false;
	bShutdownOnTimeout = false;
	dBoxTemp = 0.0;
	dTargetFermTemp = 0.0;
	dExternalTemp = 0.0;
	dFermTwoTemp = 0.0;
	dFermOneTemp = 0.0;
	pHeater = NULL;
	pCooler = NULL;

	tFermTempWatchdog = new QTimer(this);
}

Thermostat::~Thermostat() {
	// TODO Auto-generated destructor stub
}

enum ThermAlarms Thermostat::setTargetTemp(int t)
{
	double newt = (double)t;

	if (!validTemp(newt)) {
		return INVALIDTEMP;
	}
	dTargetFermTemp = newt;
	return NOERROR;
}

bool Thermostat::addHeaterGPIO(QString gpio)
{
	pHeater = new FermenterGPIO(this);
	if (pHeater->setGPIO(gpio)) {
		bHeaterEnabled = true;
		connect(pHeater, SIGNAL(valueChanged(QByteArray)), this, SLOT(heaterValueChanged(QByteArray)));
		return bHeaterEnabled;
	}
	return false;
}

bool Thermostat::addCoolerGPIO(QString gpio)
{
	pCooler = new FermenterGPIO(this);
	if (pCooler->setGPIO(gpio)) {
		bCoolerEnabled = true;
		connect(pCooler, SIGNAL(valueChanged(QByteArray)), this, SLOT(coolerValueChanged(QByteArray)));
		return bCoolerEnabled;
	}
	return false;
}

void Thermostat::heaterValueChanged(QByteArray ba)
{
	if (bCoolerIsRunning) {
		shutdown();
		emit thermostatAlarm(TWOUNITSRUNNING);
		return;
	}

	if (ba == "0") {
		bHeaterIsRunning = false;
		emit heatState(false);
	}
	if (ba == "1") {
		bHeaterIsRunning = true;
		emit heatState(true);
	}
}

void Thermostat::coolerValueChanged(QByteArray ba)
{
	if (bHeaterIsRunning) {
		shutdown();
		emit thermostatAlarm(TWOUNITSRUNNING);
		return;
	}

	if (ba == "0") {
		bCoolerIsRunning = false;
		emit heatState(false);
	}
	if (ba == "1") {
		bCoolerIsRunning = true;
		emit heatState(true);
	}
}

bool Thermostat::validTemp(double t)
{
	if (t > 45 && t < 72)
		return true;

	return false;
}

void Thermostat::currFermOneTemp(double t)
{
	if (!validTemp(t))
		return;

	dFermOneTemp = t;

	if (iActiveFermenters == 2) {
		if (t < (dFermTwoTemp - 2) || t > (dFermTwoTemp + 2)) {
			emit thermostatAlarm(FERMENTERMISMATCH);
			return;
		}
	}

	if (t < (dTargetFermTemp - 2)) {
		runHeater();
	}
	if (t > (dTargetFermTemp + 2)) {
		runCooler();
	}
	if (t < (dTargetFermTemp - 1) && bCoolerIsRunning) {
		stopCooler();
	}
	if (t > (dTargetFermTemp + 1) && bHeaterIsRunning) {
		stopHeater();
	}
}

void Thermostat::currFermTwoTemp(double t)
{
	if (!validTemp(t))
		return;

	dFermTwoTemp = t;

	if (iActiveFermenters == 2) {
		if (t < (dFermOneTemp - 2) || t > (dFermOneTemp + 2)) {
			emit thermostatAlarm(FERMENTERMISMATCH);
			return;
		}
	}

	if (t < (dTargetFermTemp - 2)) {
		runHeater();
	}
	if (t > (dTargetFermTemp + 2)) {
		runCooler();
	}
	if (t < (dTargetFermTemp - 1) && bCoolerIsRunning) {
		stopCooler();
	}
	if (t > (dTargetFermTemp + 1) && bHeaterIsRunning) {
		stopHeater();
	}
}

void Thermostat::currBoxTemp(double t)
{
	// Try to avoid overheating and starting a fire
	if (t > 80) {
		shutdown();
		emit thermostatAlarm(HIGHTEMP);
	}
	dBoxTemp = t;
}

void Thermostat::runCooler()
{
	/*
	QByteArray ba("1");

	if (bCoolerIsRunning)
		return;

	if (bHeaterIsRunning) {
		emit thermostatAlarm(HEATERRUNNING);
		return;
	}

	pCooler->setValue(ba);
	if (pCooler->checkValue("1"))
		bCoolerIsRunning = true;

	QTimer::singleShot(300000, this, SLOT(coolerSafeToShutdown()));
	*/
}

void Thermostat::runHeater()
{
	/*
	if (bHeaterIsRunning)
		return;

	if (bCoolerIsRunning) {
		emit thermostatAlarm(COOLERRUNNING);
		return;
	}

	pHeater->setValue(QByteArray("1"));
	if (pHeater->checkValue("1"))
		bHeaterIsRunning = true;
		*/
}

void Thermostat::stopCooler()
{
	/*
	if (bCoolerTimeout) {
		pCooler->setValue(QByteArray("0"));
		if (pCooler->checkValue("0")) {
			bCoolerIsRunning = false;
		}
	}
	else {
		bShutdownOnTimeout = true;
	}
	*/
}

void Thermostat::stopHeater()
{
	/*
	pHeater->setValue(QByteArray("0"));
	if (pHeater->checkValue("0")) {
		bHeaterIsRunning = false;
	}
	*/
}

void Thermostat::coolerSafeToShutdown()
{
	if (bShutdownOnTimeout) {
		stopCooler();
		bShutdownOnTimeout = false;
	}
	bCoolerTimeout = true;
}

void Thermostat::shutdown()
{
	stopCooler();
	stopHeater();
	bShutdownOnTimeout = false;
}
