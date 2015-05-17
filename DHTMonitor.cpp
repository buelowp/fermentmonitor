/*
 * DHTMonitor.cpp
 *
 *  Created on: May 17, 2015
 *      Author: pete
 */

#include "DHTMonitor.h"

DHTMonitor::DHTMonitor()
{
	values = NULL;
	metric = false;
	shm_fd = 0;
}

DHTMonitor::~DHTMonitor()
{
	munmap(values, sizeof(dht22));
	close(shm_fd);
	shm_unlink(DHT_MEM_SEGMENT);
}

bool DHTMonitor::init()
{
	if ((shm_fd = shm_open(DHT_MEM_SEGMENT, O_CREAT | O_RDONLY, 0666)) < 0) {
		qDebug() << "Unable to open shared mem segment";
		return false;
	}
	ftruncate(shm_fd, sizeof(dht22));

	if ((values = (dht22*)mmap (0, sizeof(dht22), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_INVALID) {
		qDebug() << "Unable to mmap shared memory";
		return false;
	}

	return true;
}

float getTemperature()
{
	if (!metric) {
		float t = values->temperature * 1.8 + 32;
		return t;
	}
	return values->temperature;
}
