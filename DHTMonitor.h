/*
 * DHTMonitor.h
 *
 *  Created on: May 17, 2015
 *      Author: pete
 */

#ifndef DHTMONITOR_H_
#define DHTMONITOR_H_

#include <QtCore>

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include <bbb_dht.h>

class DHTMonitor {
public:
	DHTMonitor();
	virtual ~DHTMonitor();
	bool init();
	float getTemperature();
	float getHumidity() { return values->humidity; }
	bool isValid() { return (values->mutex == MUTEX_VALID); }
	void setMetric(bool m) { metric = m; }

private:
	dht22 *values;
	int shm_fd;
	bool metric;
};

#endif /* DHTMONITOR_H_ */
