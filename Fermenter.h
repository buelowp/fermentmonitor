/*
 * Fermenter.h
 *
 *  Created on: Apr 6, 2015
 *      Author: pete
 */

#ifndef FERMENTER_H_
#define FERMENTER_H_

enum ThermAlarms {
	NOERROR = 0,
	LOWTEMP,
	HIGHTEMP,
	INVALIDTEMP,
	COOLERRUNNING,
	HEATERRUNNING,
	FERMENTERMISMATCH,
	INVALIDGPIO,
	TWOUNITSRUNNING,
};

#define IDLE			0
#define WARMING			1
#define COOLING 		2

#define HEATERRELAY		1
#define COOLERRELAY		2

#define BACKGROUND_ERROR	0
#define BACKGROUND_RUNNING	1
#define BACKGROUND_IDLE		2
#define BACKGROUND_COMPLETE	3

#endif /* FERMENTER_H_ */
