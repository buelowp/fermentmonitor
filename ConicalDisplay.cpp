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
    m_temp = 0.0;
    m_cg = 0.0;
    
	lbName = new QLabel(this);
	lbName->setText(QString("<b>%1</b>").arg(name));
	lbName->setStyleSheet(".QLabel{font: 32pt; color: black;}");
	lbTemp = new QLabel(this);
	lbTemp->setText(QString("<font style='font-family: \"Roboto\"; font-size:16pt;'>Wort Temp</font><br><font style='font-family: \"Roboto\"; font-size:36pt;'>%1</font> <font style='font-family: \"Roboto\"; font-size:20pt'>%2F</font>").arg(m_temp).arg(QChar(0xB0)));
	lbTemp->setStyleSheet(".QLabel{border-style: solid; border-radius: 5px; border-width: 1px;}");
	lbTemp->setAlignment(Qt::AlignCenter);
	m_gravity = new QLabel(this);
	m_gravity->setText(QString("<font style='font-family: \"Roboto\"; font-size:16pt;'>Estimated Gravity</font><br><font style='font-family: \"Roboto\"; font-size:36pt;'>%1</font> <font style='font-size:20pt'>gu</font>").arg(m_cg, 0, 'g', 3));
	m_gravity->setStyleSheet(".QLabel{color: black; border-style: solid; border-radius: 5px; border-width: 1px;}");
	m_gravity->setAlignment(Qt::AlignCenter);
	btnEnable = new QPushButton("Start", this);
	btnEnable->setStyleSheet(".QPushButton{font-family: \"Roboto\"; font-size: 36pt; color: black; border-radius: 5px; border-style: solid; border-width: 1px;}");
	tUpdate = new QTimer(this);
	tUpdate->setInterval(500);
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
	btnEnable = new QPushButton("Enable", this);
	tUpdate = new QTimer(this);
	tUpdate->setInterval(500);
	sw = new StopWatch();

	connect(tUpdate, SIGNAL(timeout()), this, SLOT(update()));
	connect(btnEnable, SIGNAL(pressed()), this, SLOT(enable()));
}

ConicalDisplay::~ConicalDisplay()
{
}

void ConicalDisplay::setName(QString n)
{
    name = n;
	lbName->setText(QString("<b>%1</b>").arg(n));
	sw->setName(n);
}

void ConicalDisplay::showEvent(QShowEvent* e)
{
	int divby5 = (width() / 5);
	int divby3 = (height() / 3);

	if (e->type() == QEvent::Show) {
		lbName->setGeometry(0, 0, width(), height() / 3);
		m_gravity->setGeometry(2, height() / 3, (divby5 * 2) - 4, ((divby3 * 2) - 10));
		lbTemp->setGeometry((divby5 * 2) + 2, height() / 3, (divby5 * 2) - 4, ((divby3 * 2) - 10));
		btnEnable->setGeometry((divby5 * 4) + 2, height() / 3, divby5 - 4, ((divby3 * 2) - 10));
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
		qWarning() << __PRETTY_FUNCTION__ << "Timer is running";
		sw->stop();
		tUpdate->stop();
        emit startRunning(false);
        btnEnable->setText("Start");
	}
	else {
		qWarning() << __PRETTY_FUNCTION__ << "Timer is not running";
		sw->start();
		tUpdate->start();
        emit startRunning(true);
        btnEnable->setText("Stop");
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
    m_temp = t;

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

void ConicalDisplay::updateGravity(double g)
{
    QString s = QString().setNum(g, 'f', 3);
    m_cg = g;
    
	m_gravity->setText(QString("<font style='font-size:20pt;'>Target</font><br><font style='font-size:48pt;'>%1</font> <font style='font-size:20pt'>%2F</font>").arg(s));
}
