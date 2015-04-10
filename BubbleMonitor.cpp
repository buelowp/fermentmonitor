/*
 * BubbleMonitor.cpp
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

#include "BubbleMonitor.h"

BubbleMonitor::BubbleMonitor(QString GPIO, QString Name, QObject *parent) : QObject(parent) {
	bubbles = 0;
	gpioFile = new QFile(GPIO);
	name = Name;
	watcher = NULL;
}

BubbleMonitor::~BubbleMonitor() {
	delete watcher;
}

bool BubbleMonitor::isOpen()
{
	if (!gpioFile->open(QIODevice::ReadOnly|QIODevice::Text)) {
		return false;
	}
	return true;
}

void BubbleMonitor::addEvent()
{
	QDateTime first;
	lastEvent = QDateTime::currentDateTime();

	if (events.size() == 0) {
		events.enqueue(lastEvent);
		return;
	}

	first = events.head();
	if ((lastEvent.toTime_t() - first.toTime_t()) > 60) {
		events.dequeue();
	}
	events.enqueue(lastEvent);
}

void BubbleMonitor::fileChanged(QString file)
{
	QByteArray ba;

	ba = gpioFile->readAll();

	if (ba == "1") {
		addEvent();
		emit(bubbleCount(name, ++bubbles));
	}
}

void BubbleMonitor::run()
{
	QFileSystemWatcher *fw = new QFileSystemWatcher(this);
	fw.addPath(gpioFile->fileName());
	connect(fw, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));

	//* We have to wait until fementation starts before we can do updates or spin in the check loop
	while (bubbles == 0) {
		sleep(1);
	}

	while (true) {
		QDateTime now = QDateTime::currentDateTime();

		// If we haven't had a bubble event in 12 hours, we're done
		if ((lastEvent.toTime_t() - now.toTime_t()) > 43200) {
			emit fermentationComplete(pin);
			events.clear();
		}
		sleep(1);
	}
}
