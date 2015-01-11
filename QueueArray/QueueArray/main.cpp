//
//  main.cpp
//  QueueArray
//
//  Created by Ryan Bird on 1/9/15.
//  Copyright (c) 2015 Ryan Bird. All rights reserved.
//

#define CLEAR_VALUES
#define SHRINK_QUEUE

#include <iostream>
#include <unistd.h>
#include "WrappedAroundQueue.h"



using namespace std;

// This project is meant to create a queue that is:
//    add: O(1)
//    remove: O(1)
//
//  There is time needed for resizing of queues.

int main(int argc, const char * argv[]) {
	WrapAroundQueue<int> queue;
	cout << queue << endl;
	
	for (int i = 1; i <= 8; i++) {
		queue.enqueue(i);
		cout << queue << endl;
	}
	
	cout << endl;
	
	while (queue.size()) {
		queue.dequeue();
		cout << queue << endl;
	}
	
//	test wrap around
	for (int i  = 1; i <= 4; i++)
		queue.enqueue(i);
	for (int i = 0; i < 2; i++)
		queue.dequeue();
	
	cout << endl << "Test Wrap Around" << endl;
	cout << queue << endl;
	
	for (int i = 5; i <= 6; i++) {
		queue.enqueue(i);
		cout << queue << endl;
	}
	
	cout << endl << "Test growing when start of queue is not at start of array" << endl;
	
	for (int i = 7; i <= 10; i++) {
		queue.enqueue(i);
		cout << queue << endl;
	}
	
	cout << endl;
	for (int i = 3; i <= 10; i++) {
		queue.dequeue();
		cout << queue << endl;
	}
	
//	cout << "Allow time for memory checker" << endl;
//	for (int count = 0; count < 5; count++) {
//		cout << ".";
//		sleep(3);
//	}
//	cout << endl;
	
    return 0;
}
