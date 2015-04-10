/*
 *  StopWatch.cpp
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
 *  Created on: April 10, 2015
 *      Author: Peter Buelow (goballstate at gmail)
 */

#include "StopWatch.h"

StopWatch::StopWatch() {
	bIsRunning = false;
	mStartTime = 0;
}

StopWatch::~StopWatch() {
}

void StopWatch::start()
{
	bIsRunning = true;
	mStartTime = time(NULL);
}

void StopWatch::stop()
{
	bIsRunning = false;
	mStartTime = 0;
}

void StopWatch::clear()
{
	mStartTime = 0;
}

time_t StopWatch::elapsedSeconds()
{
	time_t now = time(NULL);

	return now - mStartTime;
}

QString StopWatch::toString()
{
	if (!bIsRunning) {
		return QString("0 days, 00:00");
	}

	time_t elapsed = elapsedSeconds();

	int days = elapsed / 86400;
	elapsed = elapsed - (86400 * days);
	int hours = elapsed / 3600;
	elapsed = elapsed - (3600 * hours);
	int minutes = elapsed / 60;
	elapsed = elapsed - (60 * minutes);

	return QString("%1 days, %2:%3").arg(days).arg(hours).arg(minutes, 2, 10, QChar('0'));
}