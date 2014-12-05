//
//  BorrowHeap.h
//  Heap
//
//  Created by Ryan Bird on 11/14/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#ifndef __Heap__BorrowHeap__
#define __Heap__BorrowHeap__

#include <iostream>
#include <set>
#include <queue>
#include <algorithm>

#include "MemoryHeap.h"

#define MIN_ALLOC_SIZE 8
#define MAX_ALLOC_SIZE 128

using namespace std;


//keep track of memory ranges?
//keep track of every address?

//if set<void*>, how do I track availability(free/malloc'd memory)?
//if queue<void*>, how do I keep track of address ownership?

//have a sorted queue, if two addresses next to each other match up, it can
//be defragmented into a higher level
class SubBorrowHeap {
    int blockSize;
    
    //sorted/priority queue can help with defragmenting data
//    queue<void*> spareMemory; //keep track of availability
    MemoryHeap<void*> spareMemory;
    set<void*> allocatedMemory; //keep track of ownership
    
    SubBorrowHeap* parent;
    
public:
    SubBorrowHeap(int blockSize);
    ~SubBorrowHeap();
    
    void* malloc();
    
    //add pointers to queue of available
    void free(void*);
    
    bool contains(void*);
    
    bool borrow();
    bool borrow(SubBorrowHeap*);
    void setParent(SubBorrowHeap* parent);
    
    void addFreeMemory(void*);
    void lendFreeMemory(void*);
    void addAllocatedMemory(void*);
};

class BorrowHeap {
    SubBorrowHeap** heap; //array of pointers in order to control allocation/construction
    unsigned int heapLength;
    
    void** baseHeap;
    unsigned int baseLength;
    
    void init(unsigned int allocNumBytes);
    
public:
    BorrowHeap();
    BorrowHeap(unsigned int allocNumBytes);
    ~BorrowHeap();
    
    void* malloc(int bytes);
    void free(void*);
    
    //iterates through memory in free queues
    bool defragMemory();
};

#endif /* defined(__Heap__BorrowHeap__) */
