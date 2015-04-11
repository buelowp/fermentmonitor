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

#include <QtCore>
#include <QtWidgets>
#include <QDebug>

#include "RestServer.h"
#include "BubbleMonitor.h"
#include "TempMonitor.h"
#include "Thermostat.h"
#include "ConicalDisplay.h"

class FermentMonitor : public QFrame {
	Q_OBJECT
public:
	FermentMonitor(QWidget *parent = 0, Qt::WindowFlags f = 0);
	virtual ~FermentMonitor();
	bool init();

public slots:
	void bubbleCount(QString, int);
	void tempChange(QString, double);
	void fermentationComplete(QString);
	void thermostatAlarm(enum ThermAlarms);

signals:
	void updateLeftBPM(int);
	void updateLeftTemp(double);
	void leftConicalError();
	void updateRightBPM(int);
	void updateRightTemp(double);
	void rightConicalError();

private:
	RestServer *restHandler;
	QHash<QString, BubbleMonitor*> bubbleCounters;
	TempMonitor *temps;
	Thermostat *thermostat;
	ConicalDisplay *leftConical;
	ConicalDisplay *rightConical;
	QHBoxLayout *layout;
	QWidget *wBaseBar;
	QLabel *lbBoxTemp;
	QLabel *lbExternalTemp;
	QLabel *lbLeftTime;
	QLabel *lbRightTime;
	QTimer *tLeftTimer;
	QTimer *tRightTimer;
};

#endif /* FERMENTMONITOR_H_ */
