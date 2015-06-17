/*
 * Backlight.h
 *
 *  Created on: May 18, 2015
 *      Author: pete
 */

#ifndef BACKLIGHT_H_
#define BACKLIGHT_H_

#include <QtCore>

class Backlight : public QObject {
	Q_OBJECT
public:
	Backlight(QObject *parent = 0);
	virtual ~Backlight();
	void touchEvent();

public slots:
	void timeout();

private:
	QTimer *tEventTimeout;
	QFile *qfBacklight;
	bool bDimmed;
};

#endif /* BACKLIGHT_H_ */
