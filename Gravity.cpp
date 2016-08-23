/*
 * Gravity.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: pete
 */

#include "Gravity.h"

Gravity::Gravity(QObject *parent) : QObject(parent)
{
	m_handle = 0;
	m_temp = 59.0;
}

Gravity::~Gravity()
{
}

bool Gravity::setDevice(int dev)
{
	if ((m_handle = vl6180_initialise(dev)) < 0)
		return false;

	QTimer::singleShot(1000, this, SLOT(getMeasurement()));
	return true;
}

void Gravity::getMeasurement()
{
	int dist = get_distance(m_handle);
	double tf = 0.0;
	double hf = 0.0;
	double sg = 0.0;

	if (dist > 0) {
		hf = dist * 1.1;
		tf = ((1.313454 - (0.132674 * m_temp) + (0.00205779 * (pow(m_temp, 2))) - (0.000002627634 * (pow(m_temp, 2)))));
		sg = (hf + 1) * tf;
		emit updateGravity(sg);
	}
	QTimer::singleShot(1000, this, SLOT(getMeasurement()));
}

void Gravity::setTemperature(double t)
{
	if (t > 0)
		m_temp = t;
}

void Gravity::setName(QString n)
{
	m_name = n;
}
