//
//  EventDispatcher.cpp
//  Event Dispatcher
//
//  Created by Ryan Bird on 9/21/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

#include "EventDispatcher.h"

sem_t* EventDispatcher::semaphore = sem_open("/semaphore", O_CREAT, 0644, 0);
mutex EventDispatcher::queue_lock;
queue<void(*)()> EventDispatcher::schedule_queue;
bool EventDispatcher::taskRunning[THREADS];

EventDispatcher::EventDispatcher() {
	int value = 0;
	sem_getvalue(semaphore, &value);
	printf("Starting value: %d\n", value);
	for (int i = 0; i < THREADS; i++) {
//		pthread_create(&threads[i], NULL, method_receiver, (void*) i);
		threads[i] = new thread(method_receiver, (void*) i);
	}
}

EventDispatcher::~EventDispatcher() {
//	sem_close(semaphore);
//	sem_unlink("/semaphore");
}

void EventDispatcher::register_listener(string event_name, void (*method)() ) {
	event_listeners[event_name].insert(method);
}
bool EventDispatcher::unregister_listener(string event_name, void (*method)() ) {
	map<string, set<void(*)()> >::iterator iter = event_listeners.find(event_name);
	if (iter != event_listeners.end()) {
		set<void (*)()>& listeners = iter->second;
		return listeners.erase(method) != 0;
	}
	return false;
}

void EventDispatcher::fire_event(string event_name) {
	map<string, set<void(*)()> >::iterator iter = event_listeners.find(event_name);
	if (iter != event_listeners.end()) {
		set<void (*)()>& listeners = iter->second;
		set<void (*)()>::iterator set_iter = listeners.begin();
		while (set_iter != listeners.end()) {
			void (*listener)() = *set_iter;
			schedule_method(listener);
			
			set_iter++;
		}
	}
}

void EventDispatcher::schedule_method(void (*method)()) {
	//wait on semaphore
	
	//run method on thread
//	method();
//	run_method(method);
//	pthread_t thread;
//	pthread_create(&thread, NULL, run_method, (void*) method);
	queue_lock.lock();
	schedule_queue.push(method);
	queue_lock.unlock();
	
//	printf("POST semaphore\n");
//	int value = 0;
//	sem_getvalue(semaphore, &value);
//	printf("Pre Sem value: %d\n", value);
	
	sem_post(semaphore);
	
//	sem_getvalue(semaphore, &value);
//	printf("Post Sem value: %d\n", value);
}

void *EventDispatcher::method_receiver(void *thread_id) {
	long myId = (long) thread_id;
//	int sem_value = 0;
	int task_count = 0;
	
	while (true) {
		// wait on semaphore
//		printf("%lu: waiting on semaphore\n", myId);
		sem_wait(semaphore);
		task_count++;
//		sem_getvalue(semaphore, &sem_value);
		printf("%lu: is executing with value %d\n", myId, task_count);
		queue_lock.lock();
		void (*method)() = NULL;
		if (schedule_queue.size() > 0) {
			method = schedule_queue.front();
		}
		schedule_queue.pop();
		
		queue_lock.unlock();
		if (method != NULL)
			method();

	}
	
//	pthread_exit(NULL);
}

void *EventDispatcher::run_method(void *method_address) {
	void (*method)() = (void (*)()) method_address;
	method();
	//signal semaphore
	pthread_exit(NULL);
}

bool EventDispatcher::idle() {
//	int value = 0;
//	sem_getvalue(semaphore, &value);
	return schedule_queue.size() == 0;
}
