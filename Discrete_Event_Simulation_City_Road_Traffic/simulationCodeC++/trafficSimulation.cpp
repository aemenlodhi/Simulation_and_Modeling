/*
 * trafficSimulation.cpp
 *
 *  Created on: Feb 1, 2013
 *      Author: Aemen
 */

#include "trafficSimulation.h"

using namespace std;

int main(int argc, char** argv){

	//we proceed in units of 1 sec
	//total time units between 4 and 6 pm=7200

	if(argc!=2){
		cout<<"Usage: ./peachTreeTrafficSimulation inputSeed"<<endl;
		cout<<"Input seed required to initialize random function and record each run"<<endl;
		exit(-1);
	}

	inputSeed=atoi(argv[1]);

#if TIMECHECK
	ofstream outputTimeFile;
	outputTimeFile.open("felOutputTime.txt",ios::app);
	timespec t1, t2;
#endif

	totalTime=7200*2;

    srand(inputSeed);

	assignSizesToLanes();
	assignTraversalTimesToLanes();
	generateArrivalEvents();
	generateLightEvents();
	event eventToProcess;
    


	while(!fel.isEmpty()){

#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
#endif
				eventToProcess=extractNextEventFromFel();
#if TIMECHECK
				clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

				//unsigned long millis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

				outputTimeFile<<fel.scheduledEvents.size()<<"\t"<<diff(t1,t2).tv_sec<<"\t"<<diff(t1,t2).tv_nsec<<endl;
#endif



		#if DEBUG
				cout<<"Event T"<<eventToProcess.timeStamp<<endl;
		#endif
		if(!processEvent(eventToProcess)){
#if DEBUG
			cout<<"Error!! Event could not be processed"<<endl;
			exit(-1);
#endif
		}
	}


	for(int i=0;i<30;i++){
		if(!peachTreeLanes[i].lane.empty()){
			cout<<"Lane "<<returnQueueName(static_cast<queueNames>(i))<<" not empty"<<endl;
		}
	}

	if(!carsInSystem.empty()){
		list<int>::iterator it;
		cout<<"Cars ";
		for(it=carsInSystem.begin();it!=carsInSystem.end();it++){
			cout<<*it<<"\t";
		}
		cout<<" are still in system "<<endl;
	}

	return 0;
}
