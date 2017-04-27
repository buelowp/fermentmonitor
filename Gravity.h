/*
 *  Gravity.h
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

#ifndef Gravity_hpp
#define Gravity_hpp

#include <QtCore>
#include <vl6180.h>

class Gravity : public QObject {
    Q_OBJECT

public:
    Gravity(QObject *parent = 0);
    virtual ~Gravity();
    
    void setOG(double);
    double currentGravity();
    int currentDistance();
    bool openPort(int);
    void setMultiplier(double);

public slots:
    void updateTOF();
    void updateGravity();
    
signals:
    void gravityUpdated(double);
    void newMeasurement();
    
private:
    int m_currDistance;
    int m_filteredDistance;
    int m_lastAverage;
    double m_gravity;
    double m_sg;
    double m_multiplier;
    
    vl6180 m_handle;
    
    QTimer *m_update;
    QStack<int> m_distances;
    
};

#endif /* Gravity_hpp */
