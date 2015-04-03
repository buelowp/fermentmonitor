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

#define IDLE	0
#define WARMING	1
#define COOLING 2

#define HIGHTEMPALARM	0
#define LOWTEMPALARM	1

class Thermostat {
	Q_OBJECT

public:
	Thermostat();
	virtual ~Thermostat();
	void setCooling(double);
	void setWarming(double);

signals:
	void heatState(bool);
	void coolState(bool);
	void tempAlarm(int);

public slots:
	void setFermentOneTemp(double);
	void setFermentTwoTemp(double);
	void setBoxTemp(double);

private:
	void runCooler(double);
	void runHeater(double);
	void checkTempError(double);

	double targetTemp;
	int direction;
};

#endif /* THERMOSTAT_H_ */
