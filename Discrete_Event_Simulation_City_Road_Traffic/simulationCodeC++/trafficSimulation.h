/*
 * eventList.h
 *
 *  Created on: Feb 1, 2013
 *      Author: Aemen
 */

#ifndef TRAFFICSIMULATION_H_
#define TRAFFICSIMULATION_H_

#include<iostream>
#include<utility>
#include<algorithm>
#include<queue>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<time.h>
#include<list>
#include<fstream>
#include<ctime>

#include "namingScheme.h"
#include "car.h"
#include "road.h"
#include "light.h"
#include "event.h"
#include "futureEventsList.h"

using namespace std;

#define DEBUG 1
#define TIMECHECK 0


timespec diff(timespec start, timespec end);

int totalArrivalEventsScheduled=0;
int totalArrivalEventsProcessed=0;
int carsInSystemCurrently=0;

const int totalLanes=30;
const int totalLights=12;
int totalTime=7200;
const int redLightInterval=44;
const int greenLightInterval=30;
const float speedLimit=35;
//float timeToTraverseStraightLane=ceil(1/(35/(60*60)));
float timeToTraverseStraightLane=32;	//****temporary for debugging only*******
const int timeToTraverseIntersection=5;
int inputSeed;

list<int> carsInSystem;

struct carStats{

	int timeSpentInSystem;
	int timeSpentWaitingAtLights;
	int timeSpentBecauseQueueAtIntersectionFull;

};

road peachTreeLanes[totalLanes];
light peachTreeLights[totalLights];
light eventPeachTreeLights[totalLights];
PriorityQueue fel;
int carCounter=0;

void generateLightEvents(){

	//assume each light stays RED for 20 sec
	//assume each light stays GREEN for 40 sec
	event lightEvent;

#if TIMECHECK
	ofstream inputTimeFile;
	inputTimeFile.open("felInputTime.txt",ios::app);
	timespec t1, t2;
#endif


	for(int t=0;t<totalTime;t++){

		for(int j=0;j<totalLights;j++){
			if(eventPeachTreeLights[j].lightStatus==RED && (t-eventPeachTreeLights[j].timeUpdated)==redLightInterval){
				lightEvent.eventType=LIGHTEVENT;
				lightEvent.lightId=static_cast<lightNames>(j);
				lightEvent.timeStamp=t;
				eventPeachTreeLights[j].timeUpdated=t;
				eventPeachTreeLights[j].lightStatus=GREEN;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(lightEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif


#if DEBUG
				cout<<"Scheduled light event at t = "<<t<<endl;
#endif
			}

			if(eventPeachTreeLights[j].lightStatus==GREEN && (t-eventPeachTreeLights[j].timeUpdated)==greenLightInterval){
				lightEvent.eventType=LIGHTEVENT;
				lightEvent.lightId=static_cast<lightNames>(j);
				lightEvent.timeStamp=t;
				eventPeachTreeLights[j].timeUpdated=t;
				eventPeachTreeLights[j].lightStatus=RED;


#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(lightEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif


#if DEBUG
				cout<<"Scheduled light event at t = "<<t<<endl;
#endif
			}

		}

	}

}

event extractNextEventFromFel(){

	if(!fel.isEmpty())
		return fel.dequeue();

}

bool scheduleArrivalEvent(car carToSchedule, queueNames nextQueue, float timeToSchedule){

	event arrivalEvent;

	arrivalEvent.eventType=CAREVENT;
	arrivalEvent.carEventType=ARRIVAL;
	arrivalEvent.eventCar=carToSchedule;
	arrivalEvent.eventQueue=nextQueue;
	arrivalEvent.timeStamp=timeToSchedule;

	fel.enqueue(arrivalEvent);
	return true;
}

bool scheduleDepartureEvent(car carToSchedule, bool emptyQueue, float currentTime, float futureTime){

	event departureEvent;

	departureEvent.eventType=CAREVENT;
	departureEvent.carEventType=DEPARTURE;
	departureEvent.eventCar=carToSchedule;
	departureEvent.eventQueue=carToSchedule.currentQueue;

	if(emptyQueue)
		departureEvent.timeStamp=carToSchedule.arrivalInQueueTime+static_cast<int>(peachTreeLanes[departureEvent.eventQueue].traversalTime);
	else{
		if(currentTime!=-1)
			departureEvent.timeStamp=currentTime+timeToTraverseIntersection;
		else
			departureEvent.timeStamp=futureTime;
	}

	fel.enqueue(departureEvent);
	return true;

}

bool processDepartureEvent(event eventToProcess){

	car eventCar;
	car carToSchedule;

	ofstream delayFile;
	char delayFileName[100];
	sprintf(delayFileName,"delayFile_%d.txt",inputSeed);
	delayFile.open(delayFileName,ios::out|ios::app);

	queueNames currentLane;
	queueNames nextLane;
	lightNames currentLight;
	bool nextLaneHasCapacity;
	bool lightGreen;
	bool halfLane=false;

	currentLane=eventToProcess.eventQueue;
	eventCar=eventToProcess.eventCar;

	switch(currentLane){

		case ST_13_14_LEFT:
			nextLane=FREE_WORLD;
			currentLight=LT_13_14_STRAIGHT;
			nextLaneHasCapacity=true;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_13_14_LEFT_TURN:
			nextLane=FREE_WORLD;
			currentLight=LT_13_14_LEFT;
			nextLaneHasCapacity=true;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_13_14_RIGHT:
			nextLane=FREE_WORLD;
			currentLight=LT_13_14_STRAIGHT;
			nextLaneHasCapacity=true;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_12_13_LEFT_FH:

			if(eventCar.destination!=ST13_LEFT){
				nextLane=ST_12_13_LEFT_SH;
			}

			else{
				nextLane=ST_12_13_LEFT_TURN;
			}
			halfLane=true;
			currentLight=FREE_RUN;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			lightGreen=true;	//no light on half lane
			break;


		case ST_12_13_LEFT_SH:

			if(eventCar.destination==ST14_STRAIGHT)
				nextLane=ST_13_14_LEFT;
			else
				nextLane=ST_13_14_LEFT_TURN;

			currentLight=FREE_RUN;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			lightGreen=true;	//no light on the intersection of 13 & Peach
			break;

		case ST_12_13_LEFT_TURN:
			currentLight=FREE_RUN;
			nextLane=FREE_WORLD;
			cout<<"ERROR!!! Car was not supposed to turn left on 13th street "<<endl;
			exit(-1);
			break;

		case ST_12_13_RIGHT:

			if(eventCar.destination==ST13_RIGHT)
				nextLane=FREE_WORLD;
			else{
				nextLane=ST_13_14_RIGHT;
				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;

			}
			currentLight=FREE_RUN;
			lightGreen=true;	//no light on the intersection of 13 & Peach
			break;

		case ST_11_12_LEFT_FH:

			if(eventCar.destination==ST12_LEFT){
				nextLane=ST_11_12_LEFT_TURN;
				currentLight=FREE_RUN;
				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;

			}

			else{
				nextLane=ST_11_12_LEFT_SH;
				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;
			}
			halfLane=true;
			currentLight=FREE_RUN;
			lightGreen=true;
			break;

		case ST_11_12_LEFT_SH:

			nextLane=ST_12_13_LEFT_FH;
			currentLight=LT_11_12_STRAIGHT;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_11_12_LEFT_TURN:

			nextLane=FREE_WORLD;
			currentLight=LT_11_12_LEFT;
			nextLaneHasCapacity=true;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_11_12_RIGHT:

			if(eventCar.destination==ST12_RIGHT){
				nextLane=FREE_WORLD;
				nextLaneHasCapacity=true;
			}
			else{
				nextLane=ST_12_13_RIGHT;
				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;
			}
			currentLight=LT_11_12_STRAIGHT;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		//pasted
		case ST_10_11_LEFT_FH:

			if(eventCar.destination==ST11_LEFT){
				nextLane=ST_10_11_LEFT_TURN;
				currentLight=FREE_RUN;
				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;

			}

			else{
				nextLane=ST_10_11_LEFT_SH;
				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;
			}
			halfLane=true;
			currentLight=FREE_RUN;
			lightGreen=true;
			break;

		case ST_10_11_LEFT_SH:

			nextLane=ST_11_12_LEFT_FH;
			currentLight=LT_10_11_STRAIGHT;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_10_11_LEFT_TURN:

			nextLane=FREE_WORLD;
			currentLight=LT_10_11_LEFT;
			nextLaneHasCapacity=true;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_10_11_RIGHT:

			if(eventCar.destination==ST11_RIGHT){
				nextLane=FREE_WORLD;
				nextLaneHasCapacity=true;
			}
			else{
				nextLane=ST_11_12_RIGHT;
				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;
			}
			currentLight=LT_10_11_STRAIGHT;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		//***************Reverse side
		case ST_11_10_LEFT_FH:

			if(eventCar.destination==ST10_LEFT)
				nextLane=ST_11_10_LEFT_TURN;

			else
				nextLane=ST_11_10_LEFT_SH;

			halfLane=true;
			currentLight=FREE_RUN;
			lightGreen=true;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			break;

		case ST_11_10_LEFT_SH:

			nextLane=FREE_WORLD;
			nextLaneHasCapacity=true;
			currentLight=LT_11_10_STRAIGHT;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_11_10_LEFT_TURN:

			nextLane=FREE_WORLD;
			nextLaneHasCapacity=true;
			currentLight=LT_11_10_LEFT;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_11_10_RIGHT:
			nextLane=FREE_WORLD;
			currentLight=LT_11_10_STRAIGHT;
			nextLaneHasCapacity=true;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;


		case ST_12_11_LEFT_FH:

			if(eventCar.destination==ST11_LEFT)
				nextLane=ST_12_11_LEFT_TURN;

			else
				nextLane=ST_12_11_LEFT_SH;

			halfLane=true;
			currentLight=FREE_RUN;
			lightGreen=true;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			break;

		case ST_12_11_LEFT_SH:

			nextLane=ST_11_10_LEFT_FH;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			currentLight=LT_12_11_STRAIGHT;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_12_11_LEFT_TURN:

			nextLane=FREE_WORLD;
			nextLaneHasCapacity=true;
			currentLight=LT_12_11_LEFT;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_12_11_RIGHT:

			if(eventCar.destination==ST11_RIGHT){
				nextLane=FREE_WORLD;
				nextLaneHasCapacity=true;
				currentLight=LT_12_11_STRAIGHT;
				if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
					lightGreen=true;
				else
					lightGreen=false;
			}

			else{
				nextLane=ST_11_10_RIGHT;
				currentLight=LT_11_10_STRAIGHT;

				if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
					lightGreen=true;
				else
					lightGreen=false;

				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;


			}
			break;

		//
		case ST_13_12_LEFT_FH:

			if(eventCar.destination==ST12_LEFT)
				nextLane=ST_13_12_LEFT_TURN;

			else
				nextLane=ST_13_12_LEFT_SH;

			halfLane=true;
			currentLight=FREE_RUN;
			lightGreen=true;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			break;

		case ST_13_12_LEFT_SH:

			nextLane=ST_12_11_LEFT_FH;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			currentLight=LT_13_12_STRAIGHT;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_13_12_LEFT_TURN:

			nextLane=FREE_WORLD;
			nextLaneHasCapacity=true;
			currentLight=LT_13_12_LEFT;
			if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
				lightGreen=true;
			else
				lightGreen=false;
			break;

		case ST_13_12_RIGHT:

			if(eventCar.destination==ST12_RIGHT){
				nextLane=FREE_WORLD;
				nextLaneHasCapacity=true;
				currentLight=LT_13_12_STRAIGHT;
				if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
					lightGreen=true;
				else
					lightGreen=false;
			}

			else{
				nextLane=ST_12_11_RIGHT;
				currentLight=LT_13_12_STRAIGHT;

				if(peachTreeLights[static_cast<int>(currentLight)].lightStatus==GREEN)
					lightGreen=true;
				else
					lightGreen=false;

				if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
					nextLaneHasCapacity=true;
				else
					nextLaneHasCapacity=false;


			}
			break;

		//
		case ST_14_13_LEFT:
			nextLane=ST_13_12_LEFT_FH;
			currentLight=FREE_RUN;
			lightGreen=true;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			break;

		case ST_14_13_RIGHT:
			nextLane=ST_13_12_RIGHT;
			currentLight=FREE_RUN;
			lightGreen=true;
			if(peachTreeLanes[static_cast<int>(nextLane)].currentLoad<peachTreeLanes[static_cast<int>(nextLane)].capacity)
				nextLaneHasCapacity=true;
			else
				nextLaneHasCapacity=false;
			break;

		default:
			cout<<"ERROR!!! Lane of unknown name encountered in processing departure event at time "<<eventToProcess.timeStamp<<endl;
			exit(-1);

	}

	eventCar=eventToProcess.eventCar;

	if(nextLaneHasCapacity==true && lightGreen==true){

		carToSchedule=peachTreeLanes[currentLane].lane.front();

		if(nextLane!=FREE_WORLD){
			if(halfLane==false){

				scheduleArrivalEvent(carToSchedule,nextLane,eventToProcess.timeStamp+timeToTraverseIntersection);
			}

			else
				scheduleArrivalEvent(carToSchedule,nextLane,eventToProcess.timeStamp);
		}

		else{
			carsInSystemCurrently--;
			carsInSystem.remove(eventCar.carId);

			eventCar.departureTime=eventToProcess.timeStamp;

			if(eventCar.checkStats==true)
				delayFile<<(eventCar.departureTime-eventCar.arrivalTime)<<endl;
		}

#if DEBUG

		cout<<"Event T"<<eventToProcess.timeStamp<<" Car"<<eventToProcess.eventCar.carId<<" departed lane "<<returnQueueName(currentLane)<<" for entry into lane "<<returnQueueName(nextLane)<<endl;
		cout<<"Cars in system currently: "<<carsInSystemCurrently<<endl;
#endif

		peachTreeLanes[currentLane].lane.pop();
		peachTreeLanes[currentLane].currentLoad=peachTreeLanes[currentLane].lane.size();

		if(peachTreeLanes[currentLane].currentLoad>0){
			carToSchedule=peachTreeLanes[currentLane].lane.front();

			if((eventToProcess.timeStamp-carToSchedule.arrivalInQueueTime)>timeToTraverseStraightLane)
				scheduleDepartureEvent(carToSchedule,false,eventToProcess.timeStamp,-1);
			else
				scheduleDepartureEvent(carToSchedule,true,-1,-1);
		}

	}

	if(nextLaneHasCapacity==false && lightGreen==true){	//next lane has no capacity currently
		float futureTime=-1;
		futureTime=eventToProcess.timeStamp+timeToTraverseIntersection;
		carToSchedule=eventToProcess.eventCar;
		scheduleDepartureEvent(carToSchedule,false,-1,futureTime);

#if DEBUG
		cout<<"Event T"<<eventToProcess.timeStamp<<" Car"<<eventToProcess.eventCar.carId<<" could not depart lane "<<returnQueueName(currentLane)<<" for entry into lane "<<returnQueueName(nextLane)<<" as it is full"<<endl;
#endif

	}

	if(lightGreen==false){	//light is not GREEN
		float futureTime=-1;
		futureTime=peachTreeLights[currentLight].timeUpdated+redLightInterval+1;
		carToSchedule=eventToProcess.eventCar;
		scheduleDepartureEvent(carToSchedule,false,-1,futureTime);

#if DEBUG
		cout<<"Event T"<<eventToProcess.timeStamp<<" Car"<<eventToProcess.eventCar.carId<<" could not depart lane "<<returnQueueName(currentLane)<<" for entry into lane "<<returnQueueName(nextLane)<<" as light "<<returnLightName(currentLight)<<" is RED"<<endl;
#endif

	}

	return true;

}


bool processEvent(event eventToProcess){

	if(eventToProcess.eventType==LIGHTEVENT){

		peachTreeLights[eventToProcess.lightId].updateStatus(eventToProcess.timeStamp);
#if DEBUG
		if(peachTreeLights[eventToProcess.lightId].lightStatus==GREEN)
			cout<<"Event T"<<eventToProcess.timeStamp<<" Light "<<returnLightName(eventToProcess.lightId)<<" status GREEN"<<endl;

		else
			cout<<"Event T"<<eventToProcess.timeStamp<<" Light "<<returnLightName(eventToProcess.lightId)<<" status RED"<<endl;
#endif

		return true;
	}

	else if(eventToProcess.eventType==CAREVENT){

		car eventCar;
		eventCar=eventToProcess.eventCar;

#if DEBUG
			cout<<"Event T"<<eventToProcess.timeStamp<<" Car"<<eventToProcess.eventCar.carId<<" "<<returnCarEventName(eventToProcess.carEventType)<<" Lane "<<returnQueueName(eventToProcess.eventQueue)<<endl;
#endif


		if(eventToProcess.carEventType==ARRIVAL){	//a car needs to enter a queue

		/*	if(eventToProcess.firstArrivalEvent==true && peachTreeLanes[eventToProcess.eventQueue].lane.size()>=peachTreeLanes[eventToProcess.eventQueue].capacity){ //there is space in the lane

#if DEBUG
				cout<<"Event T"<<eventToProcess.timeStamp<<" Car"<<eventToProcess.eventCar.carId<<" "<<returnCarEventName(eventToProcess.carEventType)<<" Lane "<<returnQueueName(eventToProcess.eventQueue)<<" could not enter lane "<<returnQueueName(eventToProcess.eventQueue)<<" at time "<<eventToProcess.timeStamp<<" as lane is full"<<endl;
#endif

			}*/

			//else{
				if(eventToProcess.firstArrivalEvent){
					eventToProcess.firstArrivalEvent=false;
					eventCar.arrivalTime=eventToProcess.timeStamp;
					eventCar.inSystem=true;
					carsInSystemCurrently++;
					carsInSystem.push_back(eventToProcess.eventCar.carId);

				}

				if(eventCar.checkStats){

					cout<<"CheckStats1"<<endl;

					if(((eventCar.source==ST10_STRAIGHT || eventCar.source==ST10_LEFT) && (eventCar.destination==ST14_STRAIGHT || eventCar.destination==ST14_LEFT)) || ((eventCar.destination==ST10_STRAIGHT || eventCar.destination==ST10_LEFT) && (eventCar.source==ST14_STRAIGHT || eventCar.source==ST14_LEFT))){
						eventCar.checkStats=true;
						cout<<"CheckStats2"<<endl;
					}
					else
						eventCar.checkStats=false;
				}


				eventCar.arrivalInQueueTime=eventToProcess.timeStamp;
				eventCar.currentQueue=eventToProcess.eventQueue;
				peachTreeLanes[eventToProcess.eventQueue].lane.push(eventCar);
				peachTreeLanes[eventToProcess.eventQueue].currentLoad=peachTreeLanes[eventToProcess.eventQueue].lane.size();

#if 0
				cout<<"Car "<<eventCar.carId<<" entered lane "<<eventToProcess.eventQueue<<" at time "<<eventToProcess.timeStamp<<" Cars in queue: "<<peachTreeLanes[eventToProcess.eventQueue].lane.size()<<endl;
				cout<<"Cars in system currently: "<<carsInSystemCurrently<<endl;
#endif

				if(peachTreeLanes[eventToProcess.eventQueue].currentLoad==1){//schedule departure of car
					//schedule departure events for queue of size 1
					scheduleDepartureEvent(eventCar,true,-1,-1);
				}

			//}

			/*else{

#if DEBUG
				cout<<"Event T"<<eventToProcess.timeStamp<<" Car"<<eventToProcess.eventCar.carId<<" "<<returnCarEventName(eventToProcess.carEventType)<<" Lane "<<returnQueueName(eventToProcess.eventQueue)<<" could not enter lane "<<returnQueueName(eventToProcess.eventQueue)<<" at time "<<eventToProcess.timeStamp<<" as lane is full"<<endl;
#endif

			}*/

			return true;

		}//end if ARRIVAL event


		else if(eventToProcess.carEventType==DEPARTURE){

#if 0
			cout<<"Departure event for car "<<eventCar.carId<<" for lane "<<eventToProcess.eventQueue<<" at time "<<eventToProcess.timeStamp<<" Other cars in queue: "<<peachTreeLanes[eventToProcess.eventQueue].lane.size()-1<<endl;
#endif
			processDepartureEvent(eventToProcess);

		}


		else{

			cout<<"ERROR!! in processing CAR Event. Unknown event type encountered at time "<<eventToProcess.timeStamp<<endl;
			exit(-1);
		}


		return true;
	}

	else
		return false;
}


float expDist(float lambda){
    //Shuozhi: Added declaration here
    float exprnd;
    exprnd = (float)rand()/RAND_MAX;
	exprnd = (-1.0/lambda)*log(1-exprnd);
	return exprnd;
}

float uniDist(int max){
    //Shuozhi: Added declaration here
    float unirnd;
    unirnd = (float)rand()/RAND_MAX*max;

	return unirnd;
}


void generateArrivalEvents(){

    int cars_Arrived =0;
	int inputCars;
	float interArrivalTime;
	float currentTime=0;
	endPoints arrivalPoint;
	endPoints departurePoint;
	event arrivalEvent;
	car eventCar;
	int totalArrivalEvents=0;
    
#if TIMECHECK
	ofstream inputTimeFile;
	inputTimeFile.open("felInputTime.txt");
	timespec t1, t2;
#endif
	//arrival at 10th street
	while(currentTime<(totalTime)){

        
        
		interArrivalTime=expDist(0.3933);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

//				float carD = carDesti[cars_Arrived];
                float carD = uniDist(1);
                
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST10_STRAIGHT;
				if (carD>=0 && carD<=0.4786)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.5643)
					eventCar.destination=ST10_RIGHT;
				else if (carD<=0.5643)
					eventCar.destination=ST10_LEFT;
				else if (carD<=0.7429)
					eventCar.destination=ST11_RIGHT;
				else if (carD<=0.7965)
					eventCar.destination=ST11_LEFT;
				else if (carD<=0.8179)
					eventCar.destination=ST12_RIGHT;
				else if (carD<=0.8536)
					eventCar.destination=ST12_LEFT;
				else if (carD<=0.8607)
					eventCar.destination=ST13_RIGHT;
				else if (carD<=0.9143)
					eventCar.destination=ST14_RIGHT;
				else
					eventCar.destination=ST14_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%2)
					arrivalEvent.eventQueue=ST_10_11_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_10_11_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}

	//arrival on 14th Street
	currentTime=0;
	while(currentTime<(totalTime)){

		interArrivalTime=expDist(0.5516);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST14_STRAIGHT;
				if (carD<=0.2786)
					eventCar.destination=ST10_STRAIGHT;
				else if (carD<=0.3435)
					eventCar.destination=ST10_RIGHT;
				else if (carD<=0.3855)
					eventCar.destination=ST10_LEFT;
				else if (carD<=0.3893)
					eventCar.destination=ST11_RIGHT;
				else if (carD<=0.4008)
					eventCar.destination=ST11_LEFT;
				else if (carD<=0.4237)
					eventCar.destination=ST12_RIGHT;
				else if (carD<=0.4352)
					eventCar.destination=ST12_LEFT;
				else if (carD<=0.5268)
					eventCar.destination=ST13_RIGHT;
				else if (carD<=0.5612)
					eventCar.destination=ST14_RIGHT;
				else
					eventCar.destination=ST14_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(i%2)
					arrivalEvent.eventQueue=ST_14_13_LEFT;
				else
					arrivalEvent.eventQueue=ST_14_13_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif



				totalArrivalEvents++;


#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}

	//arrival at 10th street east
	while(currentTime<(totalTime)){

		interArrivalTime=expDist(0.3933);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST10_RIGHT;
				if (carD<=0.2424)
					eventCar.destination=ST10_STRAIGHT;
				else if (carD<=0.7879)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.8182)
					eventCar.destination=ST10_LEFT;
				else if (carD<=0.8485)
					eventCar.destination=ST11_RIGHT;
				else if (carD<=0.8788)
					eventCar.destination=ST11_LEFT;
				else if (carD<=0.8864)
					eventCar.destination=ST12_RIGHT;
				else if (carD<=0.9167)
					eventCar.destination=ST12_LEFT;
				else if (carD<=0.9243)
					eventCar.destination=ST13_RIGHT;
				else if (carD<=0.9319)
					eventCar.destination=ST14_RIGHT;
				else
					eventCar.destination=ST14_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%2)
					arrivalEvent.eventQueue=ST_10_11_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_10_11_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}

	//arrival at 10th street west
	while(currentTime<(totalTime)){

		interArrivalTime=expDist(0.3933);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST10_LEFT;
				if (carD<=0.3966)
					eventCar.destination=ST10_STRAIGHT;
				else if (carD<=0.6897)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.7276)
					eventCar.destination=ST10_RIGHT;
				else if (carD<=0.7655)
					eventCar.destination=ST11_RIGHT;
				else if (carD<=0.8)
					eventCar.destination=ST11_LEFT;
				else if (carD<=0.8345)
					eventCar.destination=ST12_RIGHT;
				else if (carD<=0.8724)
					eventCar.destination=ST12_LEFT;
				else if (carD<=0.9103)
					eventCar.destination=ST13_RIGHT;
				else if (carD<=0.9482)
					eventCar.destination=ST14_RIGHT;
				else
					eventCar.destination=ST14_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%2)
					arrivalEvent.eventQueue=ST_10_11_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_10_11_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}

	//arrival at 11th street east
	while(currentTime<(totalTime)){

		interArrivalTime=uniDist(50);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST11_RIGHT;
				if (carD<=0.1111)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.2222)
					eventCar.destination=ST10_LEFT;
				else
					eventCar.destination=ST11_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%4==0)
					arrivalEvent.eventQueue=ST_11_10_LEFT_FH;
				else if(i%4==1)
					arrivalEvent.eventQueue=ST_11_10_RIGHT;
				else if(i%4==2)
					arrivalEvent.eventQueue=ST_11_12_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_11_12_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}


	//arrival at 11th street west
	while(currentTime<(totalTime)){

		interArrivalTime=uniDist(50);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST11_LEFT;
				if (carD<=0.2653)
					eventCar.destination=ST10_STRAIGHT;
				else if (carD<=0.2857)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.7347)
					eventCar.destination=ST10_RIGHT;
				else if (carD<=0.898)
					eventCar.destination=ST10_LEFT;
				else if (carD<=0.9796)
					eventCar.destination=ST11_RIGHT;
				else
					eventCar.destination=ST14_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%4==0)
					arrivalEvent.eventQueue=ST_11_10_LEFT_FH;
				else if(i%4==1)
					arrivalEvent.eventQueue=ST_11_10_RIGHT;
				else if(i%4==2)
					arrivalEvent.eventQueue=ST_11_12_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_11_12_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}
	//arrival at 12th street east
	while(currentTime<(totalTime)){

		interArrivalTime=uniDist(50);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST12_RIGHT;
				if (carD<=0.2)
					eventCar.destination=ST10_STRAIGHT;
				else if (carD<=0.4)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.5)
					eventCar.destination=ST10_LEFT;
				else if (carD<=0.6)
					eventCar.destination=ST11_RIGHT;
				else if (carD<=0.7)
					eventCar.destination=ST11_LEFT;
				else
					eventCar.destination=ST12_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%4==0)
					arrivalEvent.eventQueue=ST_12_11_LEFT_FH;
				else if(i%4==1)
					arrivalEvent.eventQueue=ST_12_11_RIGHT;
				else if(i%4==2)
					arrivalEvent.eventQueue=ST_12_13_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_12_13_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}
	//arrival at 12th street west
	while(currentTime<(totalTime)){

		interArrivalTime=uniDist(50);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST12_LEFT;
				if (carD<=0.2)
					eventCar.destination=ST10_STRAIGHT;
				else if (carD<=0.4)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.7)
					eventCar.destination=ST10_RIGHT;
				else if (carD<=0.8)
					eventCar.destination=ST10_LEFT;
				else if (carD<=0.9)
					eventCar.destination=ST11_RIGHT;
				else
					eventCar.destination=ST11_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%4==0)
					arrivalEvent.eventQueue=ST_12_11_LEFT_FH;
				else if(i%4==1)
					arrivalEvent.eventQueue=ST_12_11_RIGHT;
				else if(i%4==2)
					arrivalEvent.eventQueue=ST_12_13_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_12_13_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}
	//arrival at 13th street east
	while(currentTime<(totalTime)){

		interArrivalTime=uniDist(50);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST13_RIGHT;
				if (carD<=0.0769)
					eventCar.destination=ST10_STRAIGHT;
				else if (carD<=0.7692)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.8461)
					eventCar.destination=ST12_LEFT;
				else
					eventCar.destination=ST14_LEFT;
				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%4==0)
					arrivalEvent.eventQueue=ST_13_12_LEFT_FH;
				else if(i%4==1)
					arrivalEvent.eventQueue=ST_13_12_RIGHT;
				else if(i%4==2)
					arrivalEvent.eventQueue=ST_13_14_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_13_14_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}

	//no car arrival at 13th street west/parkinglot
	//arrival at 14th street east
	while(currentTime<(totalTime)){

		interArrivalTime=uniDist(50);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST14_RIGHT;
				if (carD<=0.0417)
					eventCar.destination=ST10_STRAIGHT;
				else
					eventCar.destination=ST14_STRAIGHT;

				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%2)
					arrivalEvent.eventQueue=ST_14_13_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_14_13_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}

	//arrival at 14th street west
	while(currentTime<(totalTime)){

		interArrivalTime=uniDist(50);
		inputCars=rand()%2+1;

		if(inputCars>0){

			currentTime=currentTime+interArrivalTime;
			for(int i=1;i<=inputCars;i++){

				float carD = uniDist(1);//carDesti[i];
				eventCar.carId=++carCounter;
				eventCar.carType=CAR;
				eventCar.source=ST14_LEFT;
				if (carD<=0.2692)
					eventCar.destination=ST10_STRAIGHT;
				else if (carD<=0.7115)
					eventCar.destination=ST14_STRAIGHT;
				else if (carD<=0.75)
					eventCar.destination=ST10_RIGHT;
				else if (carD<=0.8365)
					eventCar.destination=ST10_LEFT;
				else if (carD<=0.8557)
					eventCar.destination=ST11_RIGHT;
				else if (carD<=0.8845)
					eventCar.destination=ST11_LEFT;
				else if (carD<=0.9037)
					eventCar.destination=ST12_RIGHT;
				else if (carD<=0.9325)
					eventCar.destination=ST12_LEFT;
				else if (carD<=0.9902)
					eventCar.destination=ST13_RIGHT;
				else
					eventCar.destination=ST14_RIGHT;

				eventCar.arrivalTime=currentTime;
				arrivalEvent.eventType=CAREVENT;
				arrivalEvent.carEventType=ARRIVAL;
				arrivalEvent.eventCar=eventCar;
				arrivalEvent.timeStamp=currentTime;
				if(arrivalEvent.timeStamp>=7200)
					eventCar.checkStats=true;

				if(i%2)
					arrivalEvent.eventQueue=ST_14_13_LEFT_FH;
				else
					arrivalEvent.eventQueue=ST_14_13_RIGHT;

				arrivalEvent.firstArrivalEvent=true;

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				fel.enqueue(arrivalEvent);
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif
				//inputTimeFile<<fel.scheduledEvents.size()<<"\t"<<millis<<endl;//(static_cast<double>(t2) - static_cast<double>(t1))<<endl;

				totalArrivalEvents++;

#if DEBUG
				cout<<"Scheduled ARRIVAL of car "<<eventCar.carId<<" at time T"<<arrivalEvent.timeStamp<<" in lane "<<returnQueueName(arrivalEvent.eventQueue)<<endl;
#endif

			}

		}

	}

#if DEBUG
	cout<<"Total "<<totalArrivalEvents<<" arrival events scheduled"<<endl;
	cout<<"Total events in FEL: "<<fel.scheduledEvents.size()<<endl;
#endif
}

void assignTraversalTimesToLanes(){


	peachTreeLanes[ST_11_10_LEFT_FH].traversalTime=peachTreeLanes[ST_10_11_LEFT_FH].traversalTime=15;
	peachTreeLanes[ST_11_10_LEFT_TURN].traversalTime=peachTreeLanes[ST_10_11_LEFT_TURN].traversalTime=peachTreeLanes[ST_11_10_LEFT_SH].traversalTime=peachTreeLanes[ST_10_11_LEFT_SH].traversalTime=15;
	peachTreeLanes[ST_11_10_RIGHT].traversalTime=peachTreeLanes[ST_10_11_RIGHT].traversalTime=30;
	peachTreeLanes[ST_12_11_LEFT_FH].traversalTime=peachTreeLanes[ST_11_12_LEFT_FH].traversalTime=15;
	peachTreeLanes[ST_12_11_LEFT_TURN].traversalTime=peachTreeLanes[ST_11_12_LEFT_TURN].traversalTime=peachTreeLanes[ST_12_11_LEFT_SH].traversalTime=peachTreeLanes[ST_11_12_LEFT_SH].traversalTime=15;
	peachTreeLanes[ST_12_11_RIGHT].traversalTime=peachTreeLanes[ST_11_12_RIGHT].traversalTime=30;
	peachTreeLanes[ST_13_12_LEFT_FH].traversalTime=peachTreeLanes[ST_12_13_LEFT_FH].traversalTime=15;
	peachTreeLanes[ST_13_12_LEFT_TURN].traversalTime=peachTreeLanes[ST_12_13_LEFT_TURN].traversalTime=peachTreeLanes[ST_13_12_LEFT_SH].traversalTime=peachTreeLanes[ST_12_13_LEFT_SH].traversalTime=15;
	peachTreeLanes[ST_13_12_RIGHT].traversalTime=peachTreeLanes[ST_12_13_RIGHT].traversalTime=30;
	peachTreeLanes[ST_13_14_LEFT_TURN].traversalTime=peachTreeLanes[ST_14_13_LEFT].traversalTime=peachTreeLanes[ST_13_14_LEFT].traversalTime=30;
	peachTreeLanes[ST_14_13_RIGHT].traversalTime=peachTreeLanes[ST_13_14_RIGHT].traversalTime=30;

}

void assignSizesToLanes(){

	peachTreeLanes[ST_11_10_LEFT_FH].capacity=peachTreeLanes[ST_10_11_LEFT_FH].capacity=13;
	peachTreeLanes[ST_11_10_LEFT_TURN].capacity=peachTreeLanes[ST_10_11_LEFT_TURN].capacity=peachTreeLanes[ST_11_10_LEFT_SH].capacity=peachTreeLanes[ST_10_11_LEFT_SH].capacity=12;
	peachTreeLanes[ST_11_10_RIGHT].capacity=peachTreeLanes[ST_10_11_RIGHT].capacity=25;
	peachTreeLanes[ST_12_11_LEFT_FH].capacity=peachTreeLanes[ST_11_12_LEFT_FH].capacity=12;
	peachTreeLanes[ST_12_11_LEFT_TURN].capacity=peachTreeLanes[ST_11_12_LEFT_TURN].capacity=peachTreeLanes[ST_12_11_LEFT_SH].capacity=peachTreeLanes[ST_11_12_LEFT_SH].capacity=12;
	peachTreeLanes[ST_12_11_RIGHT].capacity=peachTreeLanes[ST_11_12_RIGHT].capacity=24;
	peachTreeLanes[ST_13_12_LEFT_FH].capacity=peachTreeLanes[ST_12_13_LEFT_FH].capacity=10;
	peachTreeLanes[ST_13_12_LEFT_TURN].capacity=peachTreeLanes[ST_12_13_LEFT_TURN].capacity=peachTreeLanes[ST_13_12_LEFT_SH].capacity=peachTreeLanes[ST_12_13_LEFT_SH].capacity=10;
	peachTreeLanes[ST_13_12_RIGHT].capacity=peachTreeLanes[ST_12_13_RIGHT].capacity=20;
	peachTreeLanes[ST_13_14_LEFT_TURN].capacity=peachTreeLanes[ST_14_13_LEFT].capacity=peachTreeLanes[ST_13_14_LEFT].capacity=20;
	peachTreeLanes[ST_14_13_RIGHT].capacity=peachTreeLanes[ST_13_14_RIGHT].capacity=20;


}

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

/*bool arrivalProcess(int t){

	int input=rand()%11;
	int i=1;
	car* inputCar;

#if DEBUG
			cout<<"Current time in arrival process: "<<t<<" and input is "<<input<<endl;
#endif


	if(input>0 && roads[0].size()<=50){

#if DEBUG
		cout<<input<<" cars entered road 0 at time "<<t<<endl;
#endif

		i=1;
		while(i<=input && roads[0].size()<=50){
			inputCar=new car;
			inputCar->carId=++carCounter;
			inputCar->carType=CAR;
			inputCar->arrivalTime=t;
			inputCar->source=ST10;
			inputCar->destination=ST14;
			inputCar->qId=0;
			i++;
			roads[0].push(*inputCar);

			if(roads[0].size()==1){
				scheduleDepartureEvent(0,true,t); //schedule departure event for queue0
			}

		}

	}

	input=rand()%11;
	if(input>0 && roads[7].size()<=50){
		i=1;
#if DEBUG
		cout<<input<<" cars entered road 7 at time "<<t<<endl;
#endif


		while(i<=input && roads[7].size()<=50){
			inputCar=new car;
			inputCar->carId=++carCounter;
			inputCar->carType=CAR;
			inputCar->arrivalTime=t;
			inputCar->source=ST14;
			inputCar->destination=ST10;
			inputCar->qId=7;
			i++;
			roads[7].push(*inputCar);

			if(roads[7].size()==1){
				scheduleDepartureEvent(7,true,t); //schedule departure event for queue0
			}
		}

	}

}


bool processFel(int currentTime){

	std::pair <std::multimap<int,event>::iterator, std::multimap<int,event>::iterator> ret;

	std::multimap<int,event>::iterator itF = fel.find(currentTime);

#if DEBUG
	cout<<"Current time in FEL: "<<currentTime<<endl;
#endif

	if(itF!=fel.end()){

		ret = fel.equal_range(currentTime);

		for (std::multimap<int,event>::iterator it=ret.first; it!=ret.second; ++it){
#if DEBUG
			cout<<"Current time of processing FEL: "<<currentTime<<"\n";
#endif
			//if the event is about light
			if(it->second.eventType==LIGHTEVENT){
				peachTreeLights[it->second.lightId].updateStatus();
#if DEBUG
			cout<<"Light event for light "<<it->second.lightId<<" status "<<peachTreeLights[it->second.lightId].lightStatus<<endl;
#endif

			}

			else if(it->second.eventType==CAREVENT){

#if DEBUG
			cout<<"Car event \n";
#endif

				//code processing car events goes here
				if(it->second.carEventType==DEPARTURE){

#if DEBUG
			cout<<"Processing departure for car "<<it->second.carId<<"\n";
			cout<<"Status of light[0] is: "<<peachTreeLights[0].lightStatus<<endl;
#endif

					int temp;
					if(it->second.queueId==0){	//q0

						if(roads[1].size()<10 && peachTreeLights[0].lightStatus==GREEN){
#if DEBUG
		cout<<"here 1"<<endl;
#endif

							roads[1].push(roads[0].front());
							roads[0].pop();


							if(roads[1].size()==1)
								scheduleDepartureEvent(1,true,currentTime);

							if(roads[0].size()>0)
								scheduleDepartureEvent(0,false,currentTime+1);
						}

						else if(roads[1].size()==10 && peachTreeLights[0].lightStatus==GREEN){

#if DEBUG
		cout<<"here 2"<<endl;
#endif

							scheduleDepartureEvent(0,false,currentTime+1);
						}

						else if(peachTreeLights[0].lightStatus==RED){
#if DEBUG
		cout<<"here 3"<<endl;
#endif

							temp=peachTreeLights[0].timeUpdated+20;
							scheduleDepartureEvent(0,false,temp);
						}

						else{}

					}

					if(it->second.queueId==1){	//q1
						if(roads[1].size()<10 && peachTreeLights[1].lightStatus==GREEN){
							roads[2].push(roads[1].front());
							roads[1].pop();
							if(roads[2].size()==1)
								scheduleDepartureEvent(2,true,currentTime);

							if(roads[1].size()>0)
								scheduleDepartureEvent(1,false,currentTime+1);
						}

						else if(roads[2].size()==10 && peachTreeLights[1].lightStatus==GREEN){
							scheduleDepartureEvent(1,false,currentTime+1);
						}

						else if(peachTreeLights[1].lightStatus==RED){
							temp=peachTreeLights[1].timeUpdated+20;
							scheduleDepartureEvent(1,false,temp);
						}

						else{}
					}

					if(it->second.queueId==2){	//q2
						if(roads[2].size()<10 && peachTreeLights[2].lightStatus==GREEN){
							roads[3].push(roads[2].front());
							roads[2].pop();
							if(roads[3].size()==1)
								scheduleDepartureEvent(3,true,currentTime);

							if(roads[2].size()>0)
								scheduleDepartureEvent(2,false,currentTime+1);
						}

						else if(roads[3].size()==10 && peachTreeLights[2].lightStatus==GREEN){
							scheduleDepartureEvent(2,false,currentTime+1);
						}

						else if(peachTreeLights[2].lightStatus==RED){
							temp=peachTreeLights[2].timeUpdated+20;
							scheduleDepartureEvent(2,false,temp);
						}

						else{}
					}

					if(it->second.queueId==3){	//q3
						if(peachTreeLights[3].lightStatus==GREEN){
							roads[3].pop();
						}


						else if(peachTreeLights[3].lightStatus==RED){
							temp=peachTreeLights[3].timeUpdated+20;
							scheduleDepartureEvent(3,false,temp);
						}

						else{}

					}

					if(it->second.queueId==7){	//q7
						if(roads[6].size()<10 && peachTreeLights[7].lightStatus==GREEN){
							roads[6].push(roads[7].front());
							roads[7].pop();
							if(roads[6].size()==1)
								scheduleDepartureEvent(6,true,currentTime);

							if(roads[7].size()>0)
								scheduleDepartureEvent(7,false,currentTime+1);
						}

						else if(roads[6].size()==10 && peachTreeLights[7].lightStatus==GREEN){
							scheduleDepartureEvent(7,false,currentTime+1);
						}

						else if(peachTreeLights[7].lightStatus==RED){
							temp=peachTreeLights[7].timeUpdated+20;
							scheduleDepartureEvent(7,false,temp);
						}

						else{}

					}

					if(it->second.queueId==6){	//q6
						if(roads[5].size()<10 && peachTreeLights[6].lightStatus==GREEN){
							roads[5].push(roads[6].front());
							roads[6].pop();
							if(roads[5].size()==1)
								scheduleDepartureEvent(5,true,currentTime);

							if(roads[6].size()>0)
								scheduleDepartureEvent(6,false,currentTime+1);
						}

						else if(roads[5].size()==10 && peachTreeLights[6].lightStatus==GREEN){
							scheduleDepartureEvent(6,false,currentTime+1);
						}

						else if(peachTreeLights[6].lightStatus==RED){
							temp=peachTreeLights[6].timeUpdated+20;
							scheduleDepartureEvent(6,false,temp);
						}

						else{}

					}

					if(it->second.queueId==5){	//q6
						if(roads[4].size()<10 && peachTreeLights[5].lightStatus==GREEN){
							roads[4].push(roads[5].front());
							roads[5].pop();
							if(roads[4].size()==1)
								scheduleDepartureEvent(4,true,currentTime);

							if(roads[5].size()>0)
								scheduleDepartureEvent(5,false,currentTime+1);
						}

						else if(roads[4].size()==10 && peachTreeLights[5].lightStatus==GREEN){
							scheduleDepartureEvent(5,false,currentTime+1);
						}

						else if(peachTreeLights[5].lightStatus==RED){
							temp=peachTreeLights[5].timeUpdated+20;
							scheduleDepartureEvent(5,false,temp);
						}

						else{}

					}

					if(it->second.queueId==4){	//q4
						if(peachTreeLights[4].lightStatus==GREEN){
							roads[4].pop();
						}


						else if(peachTreeLights[4].lightStatus==RED){
							temp=peachTreeLights[4].timeUpdated+20;
							scheduleDepartureEvent(4,false,temp);
						}

						else{}

					}


				}


			}

			else{

				cout<<"ERROR!!!!!!"<<endl;
				cout<<"An unkown event type encountered during processing of FEL at time "<<currentTime<<endl;
				exit(-1);
			}


		}

		return true;

	}

	else{

#if DEBUG
		//cout<<"No event scheduled for this time"<<endl;
#endif
		return false;
	}


}
*/

#endif /* EVENTLIST_H_ */
