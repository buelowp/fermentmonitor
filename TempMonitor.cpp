/*
 * TempMonitor.cpp
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

#include "TempMonitor.h"

TempMonitor::TempMonitor() {
	// TODO Auto-generated constructor stub
	devicePath = "/sys/bus/w1/devices";
	bEnabled = true;
	bMetric = false;
}

TempMonitor::~TempMonitor() {
	// TODO Auto-generated destructor stub
}

void TempMonitor::run()
{
	QHashIterator<QString,QString> i(probes);
	QString s;
	int pos;

	while (bEnabled) {
		while (i.hasNext()) {
			i.next();
			QFile f(i.value());
			f.open(QIODevice::ReadOnly);
			s = i.key();

			QByteArray ba = f.readAll();
			if ((pos = ba.indexOf("t=")) != -1) {
				pos += 2;	// skip t=
				QString temp = ba.mid(pos, 5);
				double dtemp = temp.toDouble();
				dtemp = dtemp / 1000;
				if (!bMetric) {
					dtemp = ((dtemp * 1.8) + 32);
				}
				emit probeUpdate(s, dtemp);
			}
		}
		i.toFront();
		sleep(1);
	}
}

/**
 * \func void TempMonitor::populateDeviceTree()
 * \desc Collect all 1-wire devices that are temp
 * probes. We make it possible to run this whenever
 * to try and allow for hot plugged devices, even
 * though that really isn't possible electrically.
 */
int TempMonitor::populateDeviceTree()
{
	QDir oneWire(devicePath);
	QStringList entries = oneWire.entryList();

	for (int i = 0; i < entries.size(); i++) {
		QString probe = entries.at(i);
		if (probe.left(3) == "28-") {
			QFile f(probe + "/w1_slave");
			if (f.exists()) {
				if (!probes.contains(probe)) {
					probes.insert(probe, f.fileName());
				}
			}
		}
	}
	return probes.size();
}

void TempMonitor::addDevice(QString name, QString path)
{
	QFile probe(path);

	if (probe.exists()) {
		if (!probes.contains(path)) {
			probes.insert(name, path);
		}
	}
}
