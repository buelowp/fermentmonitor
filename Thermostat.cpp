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

Thermostat::Thermostat() {
	// TODO Auto-generated constructor stub
	direction = 0;
}

Thermostat::~Thermostat() {
	// TODO Auto-generated destructor stub
}

void Thermostat::setBoxTemp(double t)
{
	switch (direction) {
	case COOLING:
		if (t > targetTemp) {
			runCooler(t);
		}
		if (t < targetTemp) {
			checkTempError(t);
		}
		break;
	case WARMING:
		if (t < targetTemp) {
			runHeater(t);
		}
		if (t > targetTemp) {
			checkTempError(t);
		}
	}
}

void Thermostat::setFermentOneTemp(double t)
{
}

void Thermostat::setFermentTwoTemp(double t)
{

}

void Thermostat::setCooling(double t)
{
	direction = COOLING;
	targetTemp = t;
}

void Thermostat::setWarming(double t)
{
	direction = WARMING;
	targetTemp = t;
}

void Thermostat::checkTempError(double t)
{
	if (t > (targetTemp + 5)) {
		emit tempAlarm(HIGHTEMPALARM);
	}
	else if (t < (targetTemp - 5)) {
		emit tempAlarm(LOWTEMPALARM);
	}
}

