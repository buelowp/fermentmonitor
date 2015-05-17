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
 *  Created on: Jan 24, 2015
 *      Author: pete
 */

#include <QtCore>
#include <QApplication>
#include "bb_dht_read.h"
#include "FermentMonitor.h"

/**
 * int main(int argc, char **argv)
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStringList argList = QApplication::arguments();
    FermentMonitor monitor;
    float humidity;
    float temperature;
    int count;
    int rval;

    while ((rval = dht_read("P8_13", DHT22, &humidity, &temperature)) != DHT_SUCCESS) {
    	if (count++ < 20)
    		continue;
    	else
    		break;
    }
    if (rval == DHT_SUCCESS) {
    	qDebug() << "Temp = " << temperature;
    	qDebug() << "Humidity = " << humidity;
    }
    else
    	qDebug() << "Returned error num " << rval;

    if (!monitor.init())
    	return -1;

    monitor.setGeometry(0, 0, 800, 480);
    monitor.show();

    return app.exec();
}
