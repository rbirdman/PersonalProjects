//
//  main.cpp
//  Event Dispatcher
//
//  Created by Ryan Bird on 9/21/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "EventDispatcher.h"

using namespace std;

void testDispatcher();
void aggressiveDispatcher();

void testListener();
void testListener2();
/*
 The point of this program is to create an object that will
 call methods on anyone who wishes to listen to a specific event.
 
 Anyone who wants to know about "mouse-clicked" or "weather_changed"
 will have their static handler called.
 
 This Dispatcher is multithreaded.
 */
int main(int argc, const char * argv[]) {
//	testDispatcher();
	aggressiveDispatcher();
	
    return 0;
}

void aggressiveDispatcher() {
	EventDispatcher events;
	
	events.register_listener("first", testListener);
	events.register_listener("second", testListener2);
	events.register_listener("both", testListener);
	events.register_listener("both", testListener2);
	
	int total_tasks = 0;
	
	for (int count = 0; count < 30; count++) {
		int value = rand() % 3;
		switch (value) {
			case 0: events.fire_event("first"); total_tasks++; break;
			case 1: events.fire_event("second"); total_tasks++; break;
			case 2: events.fire_event("both"); total_tasks += 2; break;
		}
	}
	
	printf("Done scheduling %d tasks\n", total_tasks);
	
	while (!events.idle()) {
		sleep(5);
	}
	
	printf("Scheduler is idle\n");
	
	sleep(5);
}

void testDispatcher() {
	EventDispatcher events;
	
//	sleep(1);
	
	events.register_listener("test", testListener);
	events.register_listener("test", testListener2);
	events.register_listener("even", testListener2);
	
	printf("Firing test: testListener, testListener2\n");
	
	events.fire_event("test");
	
	sleep(3);
	
	bool result = events.unregister_listener("test", testListener2);
	printf("%s\n", (result)?"success":"fail");
	
	printf("Firing test: testListener\n");
	
	events.fire_event("test");
	events.fire_event("non-existant events");
	
	sleep(2);
	
	printf("Firing even: testListener2\n");
	events.fire_event("even");
	sleep(3);
}

void testListener() {
	sleep(1);
	printf("Test listener has been run\n");
}

void testListener2() {
	sleep(2);
	printf("The second test listener has been run\n");
}