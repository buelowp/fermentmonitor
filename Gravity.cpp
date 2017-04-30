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
 */

#include "Gravity.h"

Gravity::Gravity(QObject *parent) : QObject(parent)
{
    m_currDistance = 0;
    m_filteredDistance = 0;
    m_gravity = 0.0;
    m_sg = 0.0;
    m_multiplier = 1.0;
    m_lastAverage = 0;
    m_handle = 0;
    
    m_update = new QTimer(this);
    m_update->setInterval(1000);
    
    connect(m_update, SIGNAL(timeout()), this, SLOT(updateTOF()));
    connect(this, SIGNAL(newMeasurement()), this, SLOT(updateGravity()));
}

Gravity::~Gravity()
{
    
}

void Gravity::setName(QString n)
{
	m_name = n;
}

void Gravity::setDevice(int i)
{
	m_device = i;
}

void Gravity::startRunning(bool r)
{
	/*
    if (r) {
        m_update->start();
    }
    else {
        m_update->stop();
    }
    */
}

bool Gravity::openPort(int port)
{
    if ((m_handle = vl6180_initialise(port)) < 0) {
        qWarning() << __PRETTY_FUNCTION__ << "Unable to open /dev/i2c-" << port;
        return false;
    }
    
    return true;
}

void Gravity::updateTOF()
{
    m_currDistance = get_distance(m_handle);
    if (m_currDistance < 255) {
        m_distances.push(m_currDistance);
    
        if (m_distances.size() > 50) {
            m_distances.pop();
            emit newMeasurement();
        }
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
}
