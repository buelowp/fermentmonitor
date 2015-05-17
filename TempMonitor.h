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
	Q_OBJECT
public:
	TempMonitor();
	virtual ~TempMonitor();

	void addDevice(QString, QString);
	int populateDeviceTree();
	void run();
	void setDevicePath(QString s) { devicePath = s; }
	void setMetric(bool m) { bMetric = m; }
	void setCalibration(double);

signals:
	void probeUpdate(QString, double);
	void error(QString);

private:
	QHash<QString, QString> probes;
	QString devicePath;
	bool bEnabled;
	bool bMetric;
	double calFactor;
};

#endif /* TEMPMONITOR_H_ */
