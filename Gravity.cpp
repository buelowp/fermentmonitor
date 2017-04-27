<<<<<<< HEAD
//
//  Gravity.cpp
//  fermentmonitor
//
/*
 *  Gravity.cpp
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
 *  Created on: April 26, 2017
 *      Author: Peter Buelow (goballstate at gmail)
=======
/*
 * Gravity.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: pete
>>>>>>> ae865034b8245e49193474550ad8742108979367
 */

#include "Gravity.h"

Gravity::Gravity(QObject *parent) : QObject(parent)
{
<<<<<<< HEAD
    m_currDistance = 0;
    m_filteredDistance = 0;
    m_gravity = 0.0;
    m_sg = 0.0;
    m_multiplier = 1.0;
    m_lastAverage = 0;
    
    m_update = new QTimer(this);
    m_update->setInterval(1000);
    
    connect(m_update, SIGNAL(timeout()), this, SLOT(updateTOF()));
    connect(this, SIGNAL(newMeasurement()), this, SLOT(updateGravity()));
=======
	m_handle = 0;
	m_temp = 59.0;
>>>>>>> ae865034b8245e49193474550ad8742108979367
}

Gravity::~Gravity()
{
<<<<<<< HEAD
    
}

bool Gravity::openPort(int port)
{
    if ((m_handle = vl6180_initialise(port)) < 0) {
        qWarning() << __PRETTY_FUNCTION__ << "Unable to open /dev/i2c-" << port;
        return false;
    }
    
    m_update->start();
    return true;
}

void Gravity::updateTOF()
{
    m_currDistance = get_distance(m_handle);
    m_distances.push(m_currDistance);
    
    if (m_distances.size() > 20) {
        m_distances.pop();
        emit newMeasurement();
    }
}

int Gravity::currentDistance()
{
    return m_currDistance;
}

double Gravity::currentGravity()
{
    return m_gravity;
}

void Gravity::setOG(double g)
{
    m_sg = g;
}

void Gravity::setMultiplier(double m)
{
    m_multiplier = m;
}

void Gravity::updateGravity()
{
    int avgdistance;
    
    for (int i = 0; i < m_distances.size(); i++) {
        avgdistance += m_distances[i];
    }
    avgdistance /= m_distances.size();
    if (avgdistance == m_lastAverage + 1) {
        m_lastAverage = avgdistance;
        m_gravity += m_multiplier;
        emit gravityUpdated(m_gravity);
    }
=======
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
>>>>>>> ae865034b8245e49193474550ad8742108979367
}
