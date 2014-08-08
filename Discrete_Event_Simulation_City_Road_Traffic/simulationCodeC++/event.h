/*
 * event.h
 *
 *  Created on: Feb 15, 2013
 *      Author: aemen
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "namingScheme.h"
#include "car.h"

class event{
public:
	float timeStamp;
	eeventType eventType;
	ecarEventType carEventType;
	car eventCar;
	lightNames lightId;
	queueNames eventQueue;
	bool firstArrivalEvent;
	event();

};

event::event(){

	firstArrivalEvent=false;
	timeStamp=0;
	eventType=CAREVENT;
	carEventType=DEPARTURE;
	lightId=LT_10_11_STRAIGHT;
	eventQueue=ST_10_11_LEFT_FH;
}




#endif /* EVENT_H_ */
