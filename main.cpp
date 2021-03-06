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

#include <QtCore/QtCore>
#include <QtWidgets/QApplication>
#include "FermentMonitor.h"

/**
 * int main(int argc, char **argv)
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStringList argList = QApplication::arguments();
    FermentMonitor monitor;

    app.setOverrideCursor(Qt::BlankCursor);
    if (!monitor.init()) {
    	qDebug() << "Monitor init failed";
    	return -1;
    }

    monitor.setGeometry(0, 0, 800, 480);
    monitor.show();

    return app.exec();
}
