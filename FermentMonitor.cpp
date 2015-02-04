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

FermentMonitor::FermentMonitor(QObject *parent) : QFrame(parent) {
	// TODO Auto-generated constructor stub
	therms = new TempMonitor(this);
	bubbleCounter = new BubbleMonitor(this);
}

FermentMonitor::~FermentMonitor() {
	// TODO Auto-generated destructor stub
}

void FermentMonitor::addGPIO(QString s, int g)
{
	BubbleMonitor *bm = new BubbleMonitor(s, g, this);
	connect(bc, SIGNAL(bubbleCount(QString, int)), this, SLOT(bubbleCount(QString, int)));
	connect(bc, SIGNAL(fermentationComplete(QString)), this, SLOT(fementationComplete(QString)));
	bubbleCounters.insert(g, bm);
}

void FermentMonitor::bubbleCount(QString name, int count)
{
	BubbleMonitor *bm = bubbleCounters.value(name);
	int bpm = bm->bubblesPerMinute();
}

void FermentMonitor::fermentationComplete(QString name)
{

}
