//
//  MemoryHeap.h
//  Heap
//
//  Created by Ryan Bird on 11/15/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#ifndef __Heap__MemoryHeap__
#define __Heap__MemoryHeap__

#include <iostream>

template <typename Type>
class MemoryHeap {
    Type* data;
    int mySize;
    int myCount;
    
    void resize(int newSize) {
        mySize = newSize;
        Type* newHeap = new Type[mySize];
        
        memcpy(newHeap, data, myCount * sizeof(Type));
        delete [] data;
        data = newHeap;
    }
    
    int  bubbleUp(int index, Type he)
    {
        int parent = getParent(index);
        // note: (index > 0) means there is a parent
        while ((index > 0) && (he < data[parent]))
        {
            data[index] = data[parent];
            index = parent;
            parent = getParent(index);
        }
        data[index] = he;
        return index;
    }
    
    void trickleDown(int index, Type he)
    {
        int child = getLeftChild(index);
        while (child < myCount)
        {
            if (((child + 1) < myCount) &&
                (data[child + 1] < data[child]))
            {
                child++;
            }
            data[index] = data[child];
            index = child;
            child = getLeftChild(index);
        }
        bubbleUp(index, he);
    }
    
public:
    MemoryHeap() : data(NULL), mySize(1), myCount(0) {
        data = new Type[mySize];
    }
    MemoryHeap(int _size) : data(NULL), mySize(_size), myCount(0) {
        data = new Type[mySize];
    }
    
    ~MemoryHeap() {
        delete [] data;
    }
    
    int push(Type add) {
        if (myCount == mySize)
            resize(mySize * 2 + 1);
        myCount++;
        return bubbleUp(myCount - 1, add);
    }
    
    Type pop() {
        Type result = data[0];
        myCount--;
        trickleDown(0, data[myCount]);
        return result;
    }
    
    int size() {
        return myCount;
    }
    
    int getLeftChild(int index) {
        return (index * 2) + 1;
    }
    
    int getRightChild(int index) {
        return getLeftChild(index) + 1;
    }
    
    int getParent(int index) {
        return (index - 1) / 2;
    }
    
    Type getValue(int index) {
        return data[index];
    }
    
    Type remove(int index) {
        Type result = data[index];
        myCount--;
        trickleDown(index, data[myCount]);
        
        return result;
    }
};


#endif /* defined(__Heap__MemoryHeap__) */
