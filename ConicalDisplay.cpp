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

ConicalDisplay::ConicalDisplay(QWidget *parent, Qt::WindowFlags f) : QFrame(parent, f)
{
	lbName = new QLabel(this);
	lbName->setText("<b>A fermenting beer</b>");
	lbName->setStyleSheet(".QLabel{font: 32pt; color: black;}");
	lbTemp = new QLabel(this);
	lbTemp->setText(QString("<font style='font-size:16pt;'>Wort Temp</font><br><font style='font-size:48pt; color:green'>%1</font> <font style='font-size:20pt'>%2F</font>").arg((double)75.1).arg(QChar(0xB0)));
	lbTemp->setStyleSheet(".QLabel{border-style: solid; border-radius: 5px; border-width: 1px;}");
	lbTemp->setAlignment(Qt::AlignCenter);
	m_gravity = new QLabel(this);
	m_gravity->setText(QString("<font style='font-size:16pt;'>Est Gravity</font><br><font style='font-size:36pt;'>%1</font> <font style='font-size:20pt'>gu</font>").arg((double)(1.000), 0, 'g', 3));
	m_gravity->setStyleSheet(".QLabel{color: black; border-style: solid; border-radius: 5px; border-width: 1px;}");
	m_gravity->setAlignment(Qt::AlignCenter);
	lbBPM = new QLabel(this);
	lbBPM->setText(QString("<font style='font-size:16pt;'>Bubbles Per Minute</font><br><font style='font-size: 48pt;'>%1").arg(454));
	lbBPM->setStyleSheet(".QLabel{border-style: solid; border-radius: 5px; border-width: 1px;}");
	lbBPM->setAlignment(Qt::AlignCenter);
	btnEnable = new QPushButton("Start", this);
	btnEnable->setStyleSheet(".QPushButton{font: 36pt; color: black; border-radius: 5px; border-style: solid; border-width: 1px;}");
	tUpdate = new QTimer(this);
	sw = new StopWatch();

	setFrameShape(QFrame::Box);
	setFrameStyle(QFrame::Plain);
	setStyleSheet(".QFrame{background-color: red; border-radius: 5px; border-style: solid; border-width: 1px;}");

	connect(tUpdate, SIGNAL(timeout()), this, SLOT(update()));
	connect(btnEnable, SIGNAL(pressed()), this, SLOT(enable()));
}

ConicalDisplay::ConicalDisplay(QString n, QWidget *parent, Qt::WindowFlags f) : QFrame(parent, f)
{
	name = n;
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

ConicalDisplay::~ConicalDisplay()
{
}

void ConicalDisplay::setName(QString n)
{
	lbName->setText(QString("<b>%1</b>").arg(n));
	sw->setName(n);
}

void ConicalDisplay::showEvent(QShowEvent* e)
{
	if (e->type() == QEvent::Show) {
		lbName->setGeometry(0, 0, 800, height() / 3);
		m_gravity->setGeometry(5, height() / 3, 195, (((height() / 3) * 2) - 10));
		lbTemp->setGeometry(205, height() / 3, 195, (((height() / 3) * 2) - 10));
		lbBPM->setGeometry(405, height() / 3, 235, (((height() / 3) * 2) - 10));
		btnEnable->setGeometry(645, height() / 3, 140, (((height() / 3) * 2) - 10));
	}
}

void ConicalDisplay::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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

void ConicalDisplay::updateTemp(double t)
{
	QString s;

	s = QString().setNum(t, 'f', 1);

	 if (t < 50) {
		lbTemp->setText(QString("<font style='font-size:20pt;'>Actual</font><br><font style='font-size:48pt; color:blue'>%1</font> <font style='font-size:20pt'>%2F</font>").arg(s).arg(QChar(0xB0)));
	 }
	 else if (t > 80) {
		lbTemp->setText(QString("<font style='font-size:20pt;'>Actual</font><br><font style='font-size:48pt; color:red'>%1</font> <font style='font-size:20pt'>%2F</font>").arg(s).arg(QChar(0xB0)));
	 }
	 else {
		lbTemp->setText(QString("<font style='font-size:20pt;'>Actual</font><br><font style='font-size:48pt; color:black'>%1</font> <font style='font-size:20pt'>%2F</font>").arg(s).arg(QChar(0xB0)));
	 }
}

void ConicalDisplay::updateBPM(int b)
{
	lbBPM->setNum(b);
}

void ConicalDisplay::updateGravity(double g)
{
	m_gravity->setText(QString("<font style='font-size:20pt;'>Target</font><br><font style='font-size:48pt;'>%1</font> <font style='font-size:20pt'>%2F</font>").arg((double)(g), 0, 'g', 3).arg(QChar(0xB0)));
}
