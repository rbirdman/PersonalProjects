//
//  WrappedAroundQueue.h
//  QueueArray
//
//  Created by Ryan Bird on 1/9/15.
//  Copyright (c) 2015 Ryan Bird. All rights reserved.
//

#ifndef __QueueArray__WrappedAroundQueue__
#define __QueueArray__WrappedAroundQueue__

#include <stdio.h>
#include <iostream>

using namespace std;

template <typename Type>
class WrapAroundQueue {
	Type* data;
	unsigned int capacity, count;
	
	Type *front, *end, *outOfBounds;
	
public:
	
	WrapAroundQueue(unsigned int startingCapacity = 2): data(NULL), capacity(0), count(0), front(NULL), end(NULL), outOfBounds(NULL) {
		resize(startingCapacity);
	}
	
	~WrapAroundQueue() {
		if (data)
			delete [] data;
	}
	
	bool resize(unsigned int newCapacity) {
		if (newCapacity == capacity || newCapacity < count) {
			return false;
		}
		Type* newData = new Type[newCapacity];
		
		for (unsigned int index = 0; index < count; index++) {
			newData[index] = *front;
			front++;
			if (front == outOfBounds) {
				front = data;
			}
		}
		
		if (data)
			delete [] data;
		
		data = newData;
		capacity = newCapacity;
		
		front = data;
		end = data + count;
		
		outOfBounds = data + capacity;
		return true;
	}
	
	bool enqueue(const Type& val) {
		if (capacity == count) {
			resize(capacity << 1); // * 2
//			return false; // if queue isn't resizeable
		}
		
		*end = val;
		end++;
		
		if (end == outOfBounds)
			end = data;
		
		count++;
		return true;
	}
	
	Type dequeue() {
		if (count == 0) {
			// return Type* in order to return NULL?
			return Type();
		}
		
		Type temp = *front;
#ifdef CLEAR_VALUES
		*front = Type();
#endif
		front++;
		if (front == outOfBounds) {
			front = data;
		}
		
		count--;
		
//		TODO: downsize? we don't want to go below original capacity given
//		by constructor
		
#ifdef SHRINK_QUEUE
//		if count < 1/4 * capacity
		if (count < capacity >> 2) {
			resize(capacity >> 1); // half-size
		}
#endif
		
		return temp;
	}
	
	Type top() const {
		if (count == 0) {
			// return Type* to return NULL?
			return Type();
		}
		else {
			return *front;
		}
	}
	
//	inline labels?
	unsigned int Capacity() const { return capacity; }
	unsigned int size() const { return count;}
	const Type* getData() const { return data; }
	
	Type& at(unsigned int index) const { return data[index];}
	Type& operator[](unsigned int index) const { return at(index);}
};

template <typename Type>
ostream& operator<<(ostream& out, const WrapAroundQueue<Type>& queue) {
	
	for (int i = 0; i < queue.Capacity(); i++) {
		if (i != 0) {
			out << " ";
		}
		else {
			printf("%p: ", queue.getData());
		}
		out << queue.at(i);
	}
	
	return out;
}

#endif /* defined(__QueueArray__WrappedAroundQueue__) */
