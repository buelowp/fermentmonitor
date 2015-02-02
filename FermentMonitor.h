/*
 * FermentMonitor.h
 *
 *  Created on: Feb 2, 2015
 *      Author: pete
 */

#ifndef FERMENTMONITOR_H_
#define FERMENTMONITOR_H_

#include <QtGui>

class FermentMonitor : public QFrame {
	Q_OBJECT
public:
	FermentMonitor(QObject *parent = 0);
	virtual ~FermentMonitor();

	bool init();

private:
	RestServer *restServer;

};

#endif /* FERMENTMONITOR_H_ */
