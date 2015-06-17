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

BubbleMonitor::BubbleMonitor(QString GPIO, QString Name) {
	bubbles = 0;
	gpioFile = new QFile(GPIO);
	name = Name;
	watcher = NULL;
	le = 'c';
	pTimer = new QTimer();
	connect(pTimer, SIGNAL(timeout()), this, SLOT(updateBPM()));
	pTimer->start(1000);
	bFirstEvent = false;
}

BubbleMonitor::~BubbleMonitor() {
	delete watcher;
}

bool BubbleMonitor::open()
{
	if (gpioFile->isOpen()) {
		qDebug() << gpioFile->fileName() << "is open";
		return true;
	}

	if (!gpioFile->open(QIODevice::ReadOnly|QIODevice::Text)) {
		qDebug() << "Unable to open file";
		return false;
	}
	return true;
}

QDateTime BubbleMonitor::addEvent()
{
	if (!bFirstEvent) {
		bFirstEvent = true;
		return QDateTime::currentDateTime();
	}

	events.enqueue(QDateTime::currentDateTime());

	while ((events.last().toTime_t() - events.head().toTime_t()) > 60) {
		events.dequeue();
	}
	return events.last();
}

bool BubbleMonitor::checkTimeout()
{
	if (events.size() < 2) {
		return false;
	}

	int index = events.size();
	QDateTime one = events.at(index - 2);
	QDateTime two = events.at(index - 1);
	if (two.toTime_t() - one.toTime_t() > 43200) {
		return true;
	}
	return false;
}

void BubbleMonitor::updateBPM()
{
	emit bubbleCount(name, events.size());
}

void BubbleMonitor::run()
{
	struct pollfd fds[1];
	int rc;

	fds[0].fd = gpioFile->handle();
	fds[0].events = POLLPRI;

	while (true) {
		if ((rc = poll(fds, 1, 1000)) < 0) {
			qDebug() << "Error with poll" << rc;
			continue;
		}
		if (rc == 0) {
			if (checkTimeout()) {
				qDebug() << "Timeout is true";
				emit fermentationComplete(name);
			}
		}
		if (rc > 0) {
			if (fds[0].revents & POLLPRI) {
				QByteArray ba = gpioFile->readAll();
				if (ba[0] != le) {
					le = ba[0];
				}
				QDateTime now = addEvent();
				bubbles++;
			}
		}
	}
}

void BubbleMonitor::stop()
{
}

