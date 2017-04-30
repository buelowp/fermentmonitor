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

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include <QtCore/QDebug>

#include "RestServer.h"
#include "BubbleMonitor.h"
#include "TempMonitor.h"
#include "ConicalDisplay.h"
#include "DisplayPower.h"
#include "Gravity.h"

class FermentMonitor : public QFrame {
	Q_OBJECT
public:
	FermentMonitor(QWidget *parent = 0, Qt::WindowFlags f = 0);
	virtual ~FermentMonitor();
	bool init();

public slots:
	void fermentationComplete(QString);
	void updateTemps(QString, double);
	void checkRuntime(QString);
	void leftRunning(bool);
	void rightRunning(bool);

protected:
	void showEvent(QShowEvent*);

signals:
	void updateLeftTemp(double);
	void leftConicalError();
	void updateRightTemp(double);
	void rightConicalError();
	void updateLeftGravity(double);
	void updateRighGravity(double);

private:
	RestServer *restHandler;
	TempMonitor *temps;
	ConicalDisplay *leftConical;
	ConicalDisplay *rightConical;
    DisplayPower *pDisplayPower;
    Gravity *m_leftGravity;
    Gravity *m_rightGravity;
	QHBoxLayout *layout;
	QWidget *wBaseBar;
	QLabel *lbBoxTemp;
	QLabel *lbExternalTemp;
	QLabel *m_envHumidity;
	QLabel *lbLeftTime;
	QLabel *lbRightTime;
	int m_humidity;
	int m_externalTemp;
};

#endif /* FERMENTMONITOR_H_ */
