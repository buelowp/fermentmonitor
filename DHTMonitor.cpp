/*
 * DHTMonitor.cpp
 *
 *  Created on: May 17, 2015
 *      Author: pete
 */

#include "DHTMonitor.h"

DHTMonitor::DHTMonitor()
{
	pValues = NULL;
	bMetric = false;
	shm_fd = 0;
}

DHTMonitor::~DHTMonitor()
{
	munmap(pValues, sizeof(dht22));
	close(shm_fd);
	shm_unlink(DHT_MEM_SEGMENT);
}

bool DHTMonitor::init()
{
	if ((shm_fd = shm_open("/dhtdaemon", O_CREAT|O_RDONLY, 0666)) < 0) {
		qDebug() << "Unable to open shared mem segment" << errno;
		return false;
	}
	ftruncate(shm_fd, sizeof(dht22));

	if ((pValues = (dht22*)mmap (0, sizeof(dht22), PROT_READ, MAP_SHARED, shm_fd, 0)) == MAP_FAILED) {
		qDebug() << "Unable to mmap shared memory" << errno;
		return false;
	}
	return true;
}

float DHTMonitor::getTemperature()
{
	if (pValues == NULL)
		return 0.0;

	float temp = pValues->temperature;

	temp += fCalibration;

	if (!bMetric) {
		temp = temp * 1.8 + 32;
		return temp;
	}
	return pValues->temperature;
}
