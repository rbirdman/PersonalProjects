//
//  AdvancedHeap.cpp
//  Heap
//
//  Created by Ryan Bird on 11/14/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include "AdvancedHeap.h"

AdvancedHeap::AdvancedHeap() {
    heapLength = 5;
    heap = new SubHeap*[heapLength];
    for (int i = 0; i < heapLength; i++) {
        heap[i] = new SubHeap(MIN_ALLOC_SIZE << i);
    }
}

AdvancedHeap::~AdvancedHeap() {
    for (int i = 0; i < heapLength; i++) {
        delete heap[i];
    }
    delete [] heap;
}

//returns memory that is way too big for
//malloc sometimes
void* AdvancedHeap::malloc(int bytes) {
    int index = 0;
    
    while (MIN_ALLOC_SIZE << index < bytes)
        index++;
    
    //check to see if enough room
    for (; index < heapLength; index++)
        if (heap[index]->hasFreeMemory())
            return heap[index]->malloc();
    
    return NULL;
}

void* AdvancedHeap::malloc(int bytes, int expectedBytes) {
    int index = 0;
    
    while (MIN_ALLOC_SIZE << index < bytes)
        index++;
    
    //check to see if enough room
    for (; index < heapLength; index++)
        if (heap[index]->hasFreeMemory()) {
//#ifdef TEST_DATA
            assert(MIN_ALLOC_SIZE << index == expectedBytes);
//#endif
            
            return heap[index]->malloc();
        }
    
//#ifdef TEST_DATA
    assert(expectedBytes == 0);
//#endif
    
    return NULL;
}

void AdvancedHeap::free(void* garbage) {
    for (int i = 0; i < heapLength; i++) {
        if (heap[i]->contains(garbage)) {
            heap[i]->free(garbage);
            break;
        }
    }
}

void AdvancedHeap::free(void* garbage, int expectedBytes) {
    for (int i = 0; i < heapLength; i++) {
        if (heap[i]->contains(garbage)) {
            assert(MIN_ALLOC_SIZE << i == expectedBytes);
            heap[i]->free(garbage);
            break;
        }
        else if(i == heapLength - 1)
            assert(expectedBytes == 0);
    }
}


SubHeap::SubHeap() {
    init(MIN_ALLOC_SIZE, 20);
}

SubHeap::SubHeap(int byteSize) {
    init(byteSize, 20);
}

SubHeap::SubHeap(int byteSize, int numEntries) {
    init(byteSize, numEntries);
}

void SubHeap::init(int byteSize, int numEntries) {
    //8 bytes * numEntries
    //16 bytes * numEntries
    //32 bytes * numEntries
    
    blockSize = byteSize / sizeof(void*);
    heapLenth = blockSize * numEntries;
    heap = new void*[heapLenth];
    
    freeMemory = heap;
}

SubHeap::~SubHeap() {
    delete [] heap;
}

void* SubHeap::malloc() {
    void* toReturn = NULL;
    if (contains(freeMemory)) {
        toReturn = freeMemory;
        //uint64_t is eight bytes
        //freeMemory = (uint64_t*) freeMemory + blockSize;
        freeMemory = (char*) freeMemory + sizeof(void*) * blockSize;
    }
    else if(freedMemory.size()) {
        toReturn = freedMemory.front();
        freedMemory.pop();
    }
    
    return toReturn;
}

void SubHeap::free(void* garbage) {
    freedMemory.push(garbage);
}

bool SubHeap::contains(void* pointer) {
    return heap <= pointer && pointer < heap + heapLenth;
}

bool SubHeap::hasFreeMemory() {
    return contains(freeMemory) || freedMemory.size();
}


