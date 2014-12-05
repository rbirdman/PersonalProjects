//
//  BorrowHeap.cpp
//  Heap
//
//  Created by Ryan Bird on 11/14/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include "BorrowHeap.h"

BorrowHeap::BorrowHeap() {
    init(MAX_ALLOC_SIZE * 2);
}

BorrowHeap::BorrowHeap(unsigned int allocNumBytes) {
    init(allocNumBytes);
}

void BorrowHeap::init(unsigned int allocNumBytes) {
    baseLength = allocNumBytes;
    heapLength = 0;
    
    for (int count = 0; MIN_ALLOC_SIZE << count <= MAX_ALLOC_SIZE; count++) {
        heapLength++;
    }
    
    heap = new SubBorrowHeap*[heapLength];
    for (int i = 0; i < heapLength; i++) {
        heap[i] = new SubBorrowHeap(MIN_ALLOC_SIZE << i);
        
        if (i > 0)
            heap[i-1]->setParent(heap[i]);
    }
    //add memory to highest Parent
    baseHeap = (void**) new char[baseLength];
    
    int index = heapLength - 1;
    char* address = (char*) baseHeap;
    
    while (allocNumBytes > 0) {
        int size = MIN_ALLOC_SIZE << index;
        if (size > allocNumBytes) {
            index--;
        }
        else if(allocNumBytes < MIN_ALLOC_SIZE) {
            cout << "ERROR: allocation not divisible by " << MIN_ALLOC_SIZE << endl;
            return;
        }
        else {
            heap[index]->lendFreeMemory(address);
            
            address += size;
            allocNumBytes -= size;
        }
    }
    
}

BorrowHeap::~BorrowHeap() {
    for(int i = 0; i < heapLength; i++)
        delete heap[i];
    delete [] heap;
    delete [] baseHeap;
}

void* BorrowHeap::malloc(int bytes) {
    int index = 0;
    
    while (MIN_ALLOC_SIZE << index < bytes)
        index++;
    
    void* toReturn = heap[index]->malloc();
    
    if (toReturn == NULL && defragMemory())
        toReturn = heap[index]->malloc();
    
    return toReturn;
}

void BorrowHeap::free(void* garbage) {
    for (int i = 0; i < heapLength; i++) {
        if (heap[i]->contains(garbage)) {
            heap[i]->free(garbage);
            break;
        }
    }
}

//iterates through memory in free queues
bool BorrowHeap::defragMemory() {
    return false;
}

SubBorrowHeap::SubBorrowHeap(int size) : parent(NULL) {
    blockSize = size;
    
}

SubBorrowHeap::~SubBorrowHeap() {
    //return memory to parent?
}

void* SubBorrowHeap::malloc() {
    if (spareMemory.size() == 0) {
        if(!borrow())
            return NULL;
    }
    
    //void* toReturn = spareMemory.front();
    void* toReturn = spareMemory.pop();
    allocatedMemory.insert(toReturn);
    return toReturn;
    //return toReturn;
}


void SubBorrowHeap::free(void* garbage) {
    allocatedMemory.erase(garbage);
    addFreeMemory(garbage);
}

void SubBorrowHeap::lendFreeMemory(void * pointer) {
    spareMemory.push(pointer);
}

void SubBorrowHeap::addFreeMemory(void* pointer) {
    int index = spareMemory.push(pointer);
    
    int leftIndex = spareMemory.getLeftChild(index);
    int rightIndex = spareMemory.getRightChild(index);
    int parentIndex = spareMemory.getParent(index);
    
    //defragment here
    if (leftIndex < spareMemory.size()) {
        void* leftChild = spareMemory.getValue(leftIndex);
        if ((char*)leftChild - (char*)pointer == blockSize) {
            //move memory up to next level
            int child = std::max(leftIndex, index);
            int parent = std::min(leftIndex, index);
            
            //always remove child first
            spareMemory.remove(child);
            spareMemory.remove(parent);
            this->parent->addFreeMemory(pointer);
            return;
        }
    }
    if (rightIndex < spareMemory.size()) {
        void* rightChild = spareMemory.getValue(rightIndex);
        if ((char*) rightChild - (char*)pointer == blockSize) {
            //move memory up to next level
            //move memory up to next level
            int child = std::max(rightIndex, index);
            int parent = std::min(rightIndex, index);
            
            //always remove child first
            spareMemory.remove(child);
            spareMemory.remove(parent);
            this->parent->addFreeMemory(pointer);
            return;
        }
    }
    if (parentIndex < spareMemory.size()) {
        void* parent = spareMemory.getValue(parentIndex);
        if ((char*)pointer - (char*) parent == blockSize) {
            //move memory up to next level
            //move memory up to next level
            int child = std::max(parentIndex, index);
            int parent = std::min(parentIndex, index);
            
            //always remove child first
            spareMemory.remove(child);
            spareMemory.remove(parent);
            this->parent->addFreeMemory(pointer);
            return;
        }
    }
}

void SubBorrowHeap::addAllocatedMemory(void* pointer) {
    allocatedMemory.insert(pointer);
}

bool SubBorrowHeap::contains(void* pointer) {
    return allocatedMemory.find(pointer) != allocatedMemory.end();
}

bool SubBorrowHeap::borrow() {
    if (parent != NULL)
        return parent->borrow(this);
    //else
    //request more memory from real heap?
    
    return false;
}

bool SubBorrowHeap::borrow(SubBorrowHeap* child) {
    if (spareMemory.size() == 0 && !borrow()) {
        return false;
    }
    
    //void* temp = spareMemory.front();
    void* temp = spareMemory.pop();
    
    void* temp2 = (char*)temp + blockSize / 2;
    
    child->lendFreeMemory(temp);
    child->lendFreeMemory(temp2);
    
    return true;
}
void SubBorrowHeap::setParent(SubBorrowHeap* parent) {
    this->parent = parent;
}