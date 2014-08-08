#ifndef NAMINGSCHEME_H_
#define NAMINGSCHEME_H_

#include<string>
using namespace std;

enum eeventType{
	CAREVENT,
	LIGHTEVENT
};

enum elightStatus{

	GREEN,
	RED
};

enum ecarEventType{

	DEPARTURE,
	ARRIVAL
};

//Shuozi: added 31 till end
enum queueNames{

	ST_10_11_LEFT_FH,	//0
	ST_10_11_LEFT_SH,	//1
	ST_10_11_RIGHT,		//2
	ST_11_12_LEFT_FH,	//3
	ST_11_12_LEFT_SH,	//4
	ST_11_12_RIGHT,		//5
	ST_12_13_LEFT_FH,	//6
	ST_12_13_LEFT_SH,	//7
	ST_12_13_RIGHT,		//8
	ST_13_14_LEFT,		//9
	ST_13_14_RIGHT,		//10
	ST_11_10_LEFT_FH,	//11
	ST_11_10_LEFT_SH,	//12
	ST_11_10_RIGHT,		//13
	ST_12_11_LEFT_FH,	//14
	ST_12_11_LEFT_SH,	//15
	ST_12_11_RIGHT,		//16
	ST_13_12_LEFT_FH,	//17
	ST_13_12_LEFT_SH,	//18
	ST_13_12_RIGHT,		//19
	ST_14_13_LEFT,		//20
	ST_14_13_RIGHT,		//21
	ST_10_11_LEFT_TURN,	//22
	ST_11_12_LEFT_TURN,	//23
	ST_12_13_LEFT_TURN,	//24
	ST_13_14_LEFT_TURN,	//25
	ST_11_10_LEFT_TURN,	//26
	ST_12_11_LEFT_TURN,	//27
	ST_13_12_LEFT_TURN,	//28
	ST_14_13_LEFT_TURN,	//29
	FREE_WORLD,			//30
    ST_13_14_LEFT_FH,   //31
    ST_14_13_LEFT_FH    //32

};

enum lightNames{

	LT_10_11_LEFT,		//0
	LT_10_11_STRAIGHT,	//1
	LT_11_12_LEFT,		//2
	LT_11_12_STRAIGHT,	//3
	LT_13_14_LEFT,		//4
	LT_13_14_STRAIGHT,	//5
	LT_11_10_LEFT,		//6
	LT_11_10_STRAIGHT,	//7
	LT_12_11_LEFT,		//8
	LT_12_11_STRAIGHT,	//9
	LT_13_12_LEFT,		//10
	LT_13_12_STRAIGHT,	//11
	FREE_RUN			//12

};

enum ecarType { CAR, TRUCK};

//Added ST14_RIGHT
enum endPoints { ST10_STRAIGHT, ST10_RIGHT, ST10_LEFT, ST11_RIGHT, ST11_LEFT, ST12_RIGHT, ST12_LEFT, ST13_LEFT, ST13_RIGHT, ST14_STRAIGHT, ST14_LEFT, ST14_RIGHT};

string returnEventName(eeventType toName){

	string str;
	if(toName==LIGHTEVENT)
		str="LIGHT_EVENT";
	else
		str="CAR_EVENT";

	return str;

}

string returnCarEventName(ecarEventType toName){

	string str;
	if(toName==DEPARTURE)
		str="DEPARTURE";
	else
		str="ARRIVAL";

	return str;

}

string returnLightName(lightNames toName){

	string str;

	if(toName==LT_10_11_LEFT)
		str="LT_10_11_LEFT";

	else if(toName==LT_10_11_STRAIGHT)
		str="LT_10_11_STRAIGHT";

	else if(toName==LT_11_12_LEFT)
		str="LT_11_12_LEFT";

	else if(toName==LT_11_12_STRAIGHT)
		str="LT_11_12_STRAIGHT";

	else if(toName==LT_13_14_LEFT)
		str="LT_13_14_LEFT";

	else if(toName==LT_13_14_STRAIGHT)
		str="LT_13_14_STRAIGHT";

	else if(toName==LT_11_10_LEFT)
		str="LT_11_10_LEFT";

	else if(toName==LT_11_10_STRAIGHT)
		str="LT_11_10_STRAIGHT";

	else if(toName==LT_12_11_LEFT)
		str="LT_12_11_LEFT";

	else if(toName==LT_12_11_STRAIGHT)
		str="LT_12_11_STRAIGHT";

	else if(toName==LT_13_12_LEFT)
		str="LT_13_12_LEFT";

	else if(toName==LT_13_12_STRAIGHT)
		str="LT_13_12_STRAIGHT";

	else if(toName==FREE_RUN)
		str="FREE_RUN";

	else
		str="Unrecognized";

	return str;


}

string returnQueueName(queueNames toName){

	string str;
	if(toName==ST_10_11_LEFT_FH)
		str="ST_10_11_LEFT_FH";

	else if(toName==ST_10_11_LEFT_SH)
			str="ST_10_11_LEFT_SH";

	else if(toName==ST_10_11_RIGHT)
		str="ST_10_11_RIGHT";

	else if(toName==ST_11_12_LEFT_FH)
		str="ST_11_12_LEFT_FH";

	else if(toName==ST_11_12_LEFT_SH)
			str="ST_11_12_LEFT_SH";

	else if(toName==ST_11_12_RIGHT)
		str="ST_11_12_RIGHT";

	else if(toName==ST_12_13_LEFT_FH)
		str="ST_12_13_LEFT_FH";


	else if(toName==ST_12_13_LEFT_SH)
		str="ST_12_13_LEFT_SH";

	else if(toName==ST_12_13_RIGHT)
		str="ST_12_13_RIGHT";

	else if(toName==ST_13_14_LEFT)
		str="ST_13_14_LEFT";

	else if(toName==ST_13_14_RIGHT)
		str="ST_13_14_RIGHT";

	else if(toName==ST_11_10_LEFT_FH)
		str="ST_11_10_LEFT_FH";

	else if(toName==ST_11_10_LEFT_SH)
			str="ST_11_10_LEFT_SH";

	else if(toName==ST_11_10_RIGHT)
		str="ST_11_10_RIGHT";

	else if(toName==ST_12_11_LEFT_FH)
		str="ST_12_11_LEFT_FH";

	else if(toName==ST_12_11_LEFT_SH)
		str="ST_12_11_LEFT_SH";

	else if(toName==ST_12_11_RIGHT)
		str="ST_12_11_RIGHT";

	else if(toName==ST_13_12_LEFT_FH)
		str="ST_13_12_LEFT_FH";

	else if(toName==ST_13_12_LEFT_SH)
		str="ST_13_12_LEFT_SH";

	else if(toName==ST_13_12_RIGHT)
		str="ST_13_12_RIGHT";

	else if(toName==ST_14_13_LEFT)
		str="ST_14_13_LEFT";

	else if(toName==ST_14_13_RIGHT)
		str="ST_14_13_RIGHT";

	else if(toName==ST_10_11_LEFT_TURN)
		str="ST_10_11_LEFT_TURN";

	else if(toName==ST_11_12_LEFT_TURN)
		str="ST_11_12_LEFT_TURN";

	else if(toName==ST_12_13_LEFT_TURN)
		str="ST_12_13_LEFT_TURN";

	else if(toName==ST_13_14_LEFT_TURN)
		str="ST_13_14_LEFT_TURN";

	else if(toName==ST_11_10_LEFT_TURN)
		str="ST_11_10_LEFT_TURN";

	else if(toName==ST_12_11_LEFT_TURN)
		str="ST_12_11_LEFT_TURN";

	else if(toName==ST_13_12_LEFT_TURN)
		str="ST_13_12_LEFT_TURN";

	else if(toName==ST_14_13_LEFT_TURN)
		str="ST_14_13_LEFT_TURN";

	else if(toName==FREE_WORLD)
		str="FREE_WORLD";

	else
		str="Unrecognized";

	return str;
}

#endif
