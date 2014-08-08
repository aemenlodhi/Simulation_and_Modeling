/*
 * car.h
 *
 *  Created on: Feb 15, 2013
 *      Author: aemen
 */

#ifndef CAR_H_
#define CAR_H_

#include "namingScheme.h"

class car{
public:
	ecarType carType;
	int carId;
	float arrivalTime;
	float departureTime;
	float arrivalInQueueTime;
	float departureFromQueueTime;
	float headOfQueueStartTime;
	endPoints source;
	endPoints destination;
	queueNames currentQueue;
	bool inSystem;
	bool checkStats;

	car();
};

car::car(){
	checkStats=false;
	inSystem=false;
	carType=CAR;
	carId=-1;
	arrivalTime=0;
	departureTime=0;
	source=ST10_STRAIGHT;
	destination=ST14_STRAIGHT;
	currentQueue=ST_10_11_RIGHT;

	arrivalInQueueTime=0;
	departureFromQueueTime=0;
	headOfQueueStartTime=0;
}




#endif /* CAR_H_ */
