/*
 * main.cpp
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

#include "ConicalDisplay.h"

ConicalDisplay::ConicalDisplay(QString name, QObject *parent) : QObject(parent)
{
	layout = NULL;
	lbName = new QLabel(name, this);
	lbTemp = new QLabel(this);
	lbTemp->setNum(0.0);
	lbBPM = new QLabel(this);
	lbBPM->setNum(0);
	btnEnable = new QPushButton("Enable", this);
	tUpdate = new QTimer(this);
	sw = new StopWatch();

	connect(tUpdate, SIGNAL(timeout()), this, SLOT(update()));
	connect(btnEnable, SIGNAL(pressed()), this, SLOT(enable()));
}

ConicalDisplay::~ConicalDisplay() {
	// TODO Auto-generated destructor stub
}

void ConicalDisplay::enable()
{
	if (sw->isRunning()) {
		sw->stop();
		tUpdate->stop();
	}
	else {
		sw->start();
		tUpdate->start(500);
	}
}

void ConicalDisplay::update()
{
	emit updateRuntime(sw->toString());
}

void ConicalDisplay::showEvent(QShowEvent *e)
{
	if (e->type() == QEvent::Show) {
		if (!layout) {
			layout = new QHBoxLayout(this);
			layout->addWidget(lbName);
			layout->addWidget(lbTemp);
			layout->addWidget(lbBPM);
			layout->addWidget(btnEnable);
		}
	}
}

void ConicalDisplay::error()
{
	setBackground(BACKGROUND_ERROR);
}

void ConicalDisplay::setBackground(int state)
{
	QPalette pal(palette());

	switch (state) {
	case BACKGROUND_ERROR:
		pal.setColor(QPalette::Background, Qt::red);
		break;
	case BACKGROUND_RUNNING:
		pal.setColor(QPalette::Background, Qt::blue);
		break;
	case BACKGROUND_COMPLETE:
		pal.setColor(QPalette::Background, Qt::green);
		break;
	case BACKGROUND_IDLE:
		pal.setColor(QPalette::Background, Qt::white);
		break;
	}
	setAutoFillBackground(true);
	setPalette(pal);
}
