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
	// TODO Auto-generated constructor stub
	therms = new TempMonitor(this);
	restHandler = new RestServer(80);
	thermostat = new Thermostat();
}

FermentMonitor::~FermentMonitor() {
	// TODO Auto-generated destructor stub
}

void FermentMonitor::addGPIO(QString path, QString name)
{
	BubbleMonitor *bm = new BubbleMonitor(path, name, this);
	connect(bm, SIGNAL(bubbleCount(QString, int)), this, SLOT(bubbleCount(QString, int)));
	connect(bm, SIGNAL(fermentationComplete(QString)), this, SLOT(fementationComplete(QString)));
	bubbleCounters.insert(name, bm);
}

void FermentMonitor::bubbleCount(QString name, int count)
{
	BubbleMonitor *bm = bubbleCounters.value(name);
	int bpm = bm->bubblesPerMinute();
	restHandler->setBubbleCount(name, count);
	restHandler->setBubblesPerMin(name, bpm);
}

void FermentMonitor::tempChange(QString name, double temp)
{
	 restHandler->setTemp(name, temp);
}

void FermentMonitor::fermentationComplete(QString name)
{
	restHandler->setFermentState(name, true);
}

