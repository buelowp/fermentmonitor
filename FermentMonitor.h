/*
 * FermentMonitor.h
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

#ifndef FERMENTMONITOR_H_
#define FERMENTMONITOR_H_

#include "RestServer.h"
#include "BubbleMonitor.h"
#include "TempMonitor.h"

#include <QtGui>

class FermentMonitor : public QFrame {
	Q_OBJECT
public:
	FermentMonitor(QObject *parent = 0);
	virtual ~FermentMonitor();

	void addGPIO(QString, QString);

public slots:
	void bubbleCount(QString, int);
	void tempChange(QString, double);
	void fermentationComplete(QString);

private:
	RestServer *restServer;
	QHash<QString, BubbleMonitor*> bubbleCounters;
	TempMonitor *therms;
};

#endif /* FERMENTMONITOR_H_ */
