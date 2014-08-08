/*
 * futureEventsList.h
 *
 *  Created on: Feb 15, 2013
 *      Author: aemen
 */

#ifndef FUTUREEVENTSLIST_H_
#define FUTUREEVENTSLIST_H_

#include<vector>
#include<assert.h>

using namespace std;

class PriorityQueue
{
public:

	vector<event> scheduledEvents;
    void shiftRight(int low, int high);
    void shiftLeft(int low, int high);
    bool isEmpty();

    PriorityQueue(){}

    /*Insert a new item into the priority queue*/
    void enqueue(event eventToSchedule);
    /*Get the maximum element from the priority queue*/
    event dequeue();
    /*Just for testing*/
    void print();
};

bool PriorityQueue::isEmpty(){
	if(scheduledEvents.size()==0)
		return true;
	else
		return false;
}

void PriorityQueue::enqueue(event eventToSchedule)
{
    scheduledEvents.push_back(eventToSchedule);
    shiftLeft(0, scheduledEvents.size() - 1);
    return;
}

event PriorityQueue::dequeue()
{

	assert(scheduledEvents.size() != 0);
    int last = scheduledEvents.size() - 1;
    event tmp = scheduledEvents[0];
    scheduledEvents[0] = scheduledEvents[last];
    scheduledEvents[last] = tmp;
    scheduledEvents.pop_back();
    shiftRight(0, last-1);
    return tmp;
}

void PriorityQueue::print()
{
    int size = scheduledEvents.size();
    for (int i = 0; i < size; ++i)
        cout << "("<<scheduledEvents[i].eventType<<","<<scheduledEvents[i].timeStamp<<")";
    cout << endl;
}

void PriorityQueue::shiftLeft(int low, int high)
{
    int childIdx = high;
    while (childIdx > low)
    {
        int parentIdx = (childIdx-1)/2;
        /*if child is bigger than parent we need to swap*/
        if (scheduledEvents[childIdx].timeStamp < scheduledEvents[parentIdx].timeStamp)
        {
            event tmp = scheduledEvents[childIdx];
            scheduledEvents[childIdx] = scheduledEvents[parentIdx];
            scheduledEvents[parentIdx] = tmp;
            /*Make parent index the child and shift towards left*/
            childIdx = parentIdx;
        }
        else
        {
            break;
        }
    }
    return;
}

void PriorityQueue::shiftRight(int low, int high)
{
    int root = low;
    while ((root*2)+1 <= high)
    {
        int leftChild = (root * 2) + 1;
        int rightChild = leftChild + 1;
        int swapIdx = root;
        /*Check if root is greaater than left child*/
        if (scheduledEvents[swapIdx].timeStamp > scheduledEvents[leftChild].timeStamp)
        {
            swapIdx = leftChild;
        }
        /*If right child exists check if it is less than current root*/
        if ((rightChild <= high) && (scheduledEvents[swapIdx].timeStamp > scheduledEvents[rightChild].timeStamp))
        {
            swapIdx = rightChild;
        }
        /*Make the biggest element of root, left and right child the root*/
        if (swapIdx != root)
        {
            event tmp = scheduledEvents[root];
            scheduledEvents[root] = scheduledEvents[swapIdx];
            scheduledEvents[swapIdx] = tmp;
            /*Keep shifting right and ensure that swapIdx satisfies
            heap property aka left and right child of it is smaller than
            itself*/
            root = swapIdx;
        }
        else
        {
            break;
        }
    }
    return;
}





#endif /* FUTUREEVENTSLIST_H_ */
