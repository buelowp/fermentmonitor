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
#include <errno.h>

#include <bbb_dht.h>

class DHTMonitor {
public:
	DHTMonitor();
	virtual ~DHTMonitor();
	bool init();
	float getTemperature();
	float getHumidity() 
	{ 
		if (pValues)
			return pValues->humidity; 
		else
			return 0.0;
	}
	bool isValid() 
	{ 
		if (pValues) 
			return (pValues->mutex == MUTEX_VALID); 
		else
			return false;
	}
	void setMetric(bool m) { bMetric = m; }
	void setCalibration(float c) { fCalibration = c; }

private:
	dht22 *pValues;
	int shm_fd;
	bool bMetric;
	float fCalibration;
};

#endif /* DHTMONITOR_H_ */
