/*
 * TempMonitor.h
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

#ifndef TEMPMONITOR_H_
#define TEMPMONITOR_H_

#include <QtCore>

class TempMonitor : public QThread {
public:
	TempMonitor(QObject *parent = 0);
	virtual ~TempMonitor();

	void run();
	void setDevicePath(QString s) { devicePath = s; }

signals:
	void probeUpdate(QString, double);
	void error(QString);

private:
	int populateDeviceTree();

	QHash<QString, QFile> probes;
	QString devicePath;
	bool bEnabled;
};

#endif /* TEMPMONITOR_H_ */
