//
//  MyQueue.h
//  lab6-AVLTree
//
//  Created by Ryan Bird on 3/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef lab6_AVLTree_MyQueue_h
#define lab6_AVLTree_MyQueue_h

template <typename Type>
class Queue {
	
	class Node {
	public:
		Node* next;
		Type value;
		
		Node() : next(NULL) {
		}
		
		Node(const Type& val) : next(NULL), value(val) {
		}
	};
	
	Node* head;
	Node* tail;
	
public:
	
	Queue() : head(NULL), tail(NULL) {
	}
	
	Type pop() {
		if(head != NULL) {
			Node* temp = head;
			head = head->next;
			
			Type val = temp->value;
			delete temp;
			
			return val;
		}
		
		return Type();
	}
	
	void add(const Type& toAdd) {
		Node* temp = new Node(toAdd);
		
		if (tail == NULL)
			head = tail = temp;
		else {
			tail->next = temp;
			tail = temp;
		}
	}
	
	bool empty() {
		return head == NULL;
	}
	
};

#endif
