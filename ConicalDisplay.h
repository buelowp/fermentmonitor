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
#include <QtGui>
#include "StopWatch.h"
#include "Fermenter.h"

class ConicalDisplay : public QFrame {
	Q_OBJECT
public:
	ConicalDisplay(QString, QObject *parent = 0);
	virtual ~ConicalDisplay();

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

private:
	void setBackground(int);

	QHBoxLayout *layout;
	QLabel *lbTemp;
	QLabel *lbBPM;
	QLabel *lbName;
	QPushButton *btnEnable;
	QTimer *tUpdate;
	StopWatch *sw;
};

#endif /* CONICALDISPLAY_H_ */
