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

#ifndef THERMOSTAT_H_
#define THERMOSTAT_H_

#include <QtCore>

#include "Fermenter.h"
#include "FermenterGPIO.h"

class Thermostat : public QObject {
	Q_OBJECT

public:
	Thermostat(QObject *parent = 0);
	virtual ~Thermostat();
	void setDirection(int);
	bool addCoolerGPIO(QString);
	bool addHeaterGPIO(QString);
	ThermAlarms setTargetTemp(double);
	ThermAlarms setTargetTemp(int);
	void shutdown();
	void addFermenter() { iActiveFermenters++; }
	void removeFermenter() { iActiveFermenters--; }

signals:
	void heatState(bool);
	void coolState(bool);
	void thermostatAlarm(enum ThermAlarms);

public slots:
	void currBoxTemp(double);
	void currFermOneTemp(double);
	void currFermTwoTemp(double);
	void coolerValueChange(QByteArray);
	void heaterValueChange(QByteArray);
	void coolerSafeToShutdown();

private:
	void runCooler();
	void runHeater();
	void stopCooler();
	void stopHeater();
	void checkTempError(double);
	bool validTemp(double);
	bool coolerIsRunning();
	bool heaterIsRunning();

	QTimer *tFermTempWatchdog;
	FermenterGPIO *pHeater;
	FermenterGPIO *pCooler;
	double dTargetFermTemp;
	double dBoxTemp;
	double dExternalTemp;
	double dFermOneTemp;
	double dFermTwoTemp;
	bool bCoolerIsRunning;
	bool bHeaterIsRunning;
	bool bHeaterEnabled;
	bool bCoolerEnabled;
	bool bCoolerTimeout;
	bool bShutdownOnTimeout;
	int iActiveFermenters;
};

#endif /* THERMOSTAT_H_ */
