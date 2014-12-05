//
//  AdvancedHeap.h
//  Heap
//
//  Created by Ryan Bird on 11/14/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#ifndef __Heap__AdvancedHeap__
#define __Heap__AdvancedHeap__

#include <iostream>
#include <queue>

#include <assert.h>

using namespace std;

//class of indexes in order to split/join memory?

//class Index ()
//    Index* split() //array of indexes of smaller sizes to be given to smaller collection

//sizeof(void*)
#define MIN_ALLOC_SIZE 8
#define MAX_ALLOC_SIZE 128


//footers/headers? (one byte each?)
//no, we already know the size of each chunk
class SubHeap {
    void** heap;
    unsigned int heapLenth;
    int blockSize;
    
    void* freeMemory;
    queue<void*> freedMemory;
    
    void init(int byteSize, int numEntries);
    
public:
    
    SubHeap();
    SubHeap(int byteSize);
    SubHeap(int byteSize, int numEntries);
    ~SubHeap();
    
    void* malloc();
    
    //add pointers to queue of available
    void free(void*);
    
    bool contains(void*);
    bool hasFreeMemory();
    
    //option:
    //split
    //borrow
};

//two techniques:
//  preset sizes

//  borrowing technique one large allocation, smaller allocations
//  can borrow from the size just above (borrow 1 index split into two smaller halves)
//  when completely out of memory, defragment the free memory to see if there's enough room

class AdvancedHeap {
    //SubHeap eightBytes;
    
    SubHeap** heap; //array of pointers in order to control allocation/construction
    int heapLength;
    
    //[0]: 8 bytes
    //[1]: 16 bytes
    //[2]: 32 bytes
    //[3]: 64 bytes
    //[4]: 128 bytes
    
public:
    AdvancedHeap();
    ~AdvancedHeap();
    
    void* malloc(int bytes);
    void* malloc(int bytes, int expectedBytes);
    void free(void*);
    void free(void*, int expectedBytes);
    
};

#endif /* defined(__Heap__AdvancedHeap__) */
