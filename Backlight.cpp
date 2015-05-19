/*
 * Backlight.cpp
 *
 *  Created on: May 18, 2015
 *      Author: pete
 */

#include "Backlight.h"

Backlight::Backlight(QObject *parent) : QObject(parent)
{
	bDimmed = false;
	tEventTimeout = new QTimer();
	connect(tEventTimeout, SIGNAL(timeout()), this, SLOT(timeout()));
	tEventTimeout->setInterval(60000);
	tEventTimeout->setSingleShot(true);

	QDir dir("/sys/class/backlight");
	QStringList filelist = dir.entryList();
	for (int i = 0; i < filelist.size(); i++) {
		QString s = filelist.at(i);
		if (s.contains("backlight")) {
			qfBacklight = new QFile(dir.filePath(s));
		}
	}
}

Backlight::~Backlight()
{
	delete tEventTimeout;
}

void Backlight::timeout()
{
	if (!bDimmed) {
		qfBacklight->write("50");
		bDimmed = true;
		tEventTimeout->setInterval(60000);
		tEventTimeout->setSingleShot(true);
	}
	else {
		qfBacklight->write("0");
	}
}

void Backlight::touchEvent()
{
	qfBacklight->write("100");
	bDimmed = false;
	tEventTimeout->setInterval(60000);
	tEventTimeout->setSingleShot(true);
}
