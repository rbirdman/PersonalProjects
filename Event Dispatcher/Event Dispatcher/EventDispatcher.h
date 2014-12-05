//
//  EventDispatcher.h
//  Event Dispatcher
//
//  Created by Ryan Bird on 9/21/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

#ifndef __Event_Dispatcher__EventDispatcher__
#define __Event_Dispatcher__EventDispatcher__

#include <stdio.h>
#include <map>
//#include <vector>
#include <set>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <queue>

#include <thread>
#include <mutex>

using namespace std;

//#define THREADS 2
//#define THREADS 4
#define THREADS 6
// TODO: Make multi-threaded

class EventDispatcher {
//	event_name => vector of methods
	map<string, set<void (*)()> > event_listeners;
	
//	This method allows for multi-threaded capabilities
	void schedule_method(void (*)());
	static void *method_receiver(void *thread_id);
	
	static void *run_method(void *method_address);
	
//	pthread_t threads[THREADS];
	thread* threads[THREADS];
	static sem_t* semaphore;
	static mutex queue_lock;
	static bool taskRunning[THREADS];
	
	static queue<void (*)()> schedule_queue;
	
	
public:
	EventDispatcher();
	~EventDispatcher();
	
	void register_listener(string, void() );
	bool unregister_listener(string, void() );
	
	void fire_event(string);
	bool idle();
};

#endif /* defined(__Event_Dispatcher__EventDispatcher__) */
