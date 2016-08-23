/*
 * Gravity.h
 *
 *  Created on: Aug 23, 2016
 *      Author: pete
 */

#ifndef GRAVITY_H_
#define GRAVITY_H_

#include <QtCore/QtCore>
#include <math.h>
#include <vl6180_pi/vl6180_pi.h>

class Gravity : public QObject {
	Q_OBJECT
public:
	Gravity(QObject *parent = 0);
	virtual ~Gravity();
	bool setDevice(int);
	void setName(QString);

signals:
	void updateGravity(double);

public slots:
	void setTemperature(double);

private slots:
	void getMeasurement();

private:
	vl6180 m_handle;
	double m_temp;
	QString m_name;
};

#endif /* GRAVITY_H_ */
