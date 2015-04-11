/*
 * ConicalDisplay.h
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
 *  Created on: April 10, 2015
 *      Author: Peter Buelow (goballstate at gmail)
 */

#ifndef CONICALDISPLAY_H_
#define CONICALDISPLAY_H_

#include <QtCore>
#include <QtWidgets>

#include "StopWatch.h"
#include "Fermenter.h"

class ConicalDisplay : public QFrame {
	Q_OBJECT
public:
	ConicalDisplay(QWidget *parent = 0, Qt::WindowFlags f = 0);
	ConicalDisplay(QString, QWidget *parent = 0, Qt::WindowFlags f = 0);
	virtual ~ConicalDisplay();
	void setName(QString n);
	void setHoldTemp(double);

public slots:
	void updateBPM(int);
	void updateTemp(double);
	void error();
	void enable();
	void update();

signals:
	void updateRuntime(QString);

protected:
	void showEvent(QShowEvent*);
	void paintEvent(QPaintEvent*);

private:
	void setBackground(int);

	QLabel *lbTemp;
	QLabel *lbHold;
	QLabel *lbBPM;
	QLabel *lbName;
	QPushButton *btnEnable;
	QTimer *tUpdate;
	StopWatch *sw;
	QString name;
};

#endif /* CONICALDISPLAY_H_ */
