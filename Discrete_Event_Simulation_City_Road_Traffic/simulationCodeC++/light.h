/*
 * light.h
 *
 *  Created on: Feb 15, 2013
 *      Author: aemen
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "namingScheme.h"

class light{
public:
	lightNames lightName;
	elightStatus lightStatus;
	float timeUpdated;
	light();
	void updateStatus(float);
};

light::light(){

	lightName=LT_10_11_STRAIGHT;
	lightStatus=RED;
	timeUpdated=0;
}

void light::updateStatus(float timeStamp){
	if(lightStatus==RED)
		lightStatus=GREEN;
	else
		lightStatus=RED;
	timeUpdated=timeStamp;
}




#endif /* LIGHT_H_ */
