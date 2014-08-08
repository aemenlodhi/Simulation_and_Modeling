/*
 * road.h
 *
 *  Created on: Feb 15, 2013
 *      Author: aemen
 */

#ifndef ROAD_H_
#define ROAD_H_

#include "namingScheme.h"

const float lengthOfStraightLane=1;
const float lengthOfTurningLane=0.5;

class road{
public:

	queue<car> lane;
	unsigned int capacity;
	unsigned int traversalTime;
	unsigned int currentLoad;
	queueNames laneName;
	float laneLength;

	//statistics for queue
	int totalCarsPassed;
	double avgLoadOnQueue;
	queue<double> delayOnQueue;

	road();

};

road::road(){
	capacity=10;
	currentLoad=0;
	laneName=ST_10_11_LEFT_FH;
	laneLength=lengthOfStraightLane;
	traversalTime=10;


	totalCarsPassed=0;
	avgLoadOnQueue=0;
}




#endif /* ROAD_H_ */
