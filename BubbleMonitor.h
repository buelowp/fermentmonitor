/*
 * BubbleMonitor.h
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
 *  Created on: Feb 2, 2015
 *      Author: pete
 */

#ifndef BUBBLEMONITOR_H_
#define BUBBLEMONITOR_H_

#include <QtCore/QtCore>

class BubbleMonitor : public QThread {
	Q_OBJECT
public:
	BubbleMonitor(QString GPIO, QString name);
	virtual ~BubbleMonitor();

	bool isOpen();
	void run();
	int bubblesPerMinute() { return events.size(); }
	QString getName() { return name; }

signals:
	void bubbleCount(QString, int);
	void fermentationComplete(QString);
	void error(QString);

public slots:
	void stop();

protected slots:
	void fileChanged(QString);

private:
	void addEvent();

	int bubbles;
	QString name;
	QFile *gpioFile;
	QFileSystemWatcher* watcher;
	QQueue<QDateTime> events;
	QDateTime lastEvent;
};

#endif /* BUBBLEMONITOR_H_ */
