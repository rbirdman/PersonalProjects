//
//  Heap.h
//  Heap
//
//  Created by Ryan Bird on 11/11/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#ifndef __Heap__Heap__
#define __Heap__Heap__

#include <iostream>
#include <queue>

#include <assert.h>

using namespace std;


//typename Type;
#define DEFAULT_SIZE 20

//#define TEST_DATA

template <typename Type>
class Heap {
    //TODO: void* heap with different sizes
    Type* heap;
    int heapSize;
    
    queue<int> availableIndexes;
    
    void init(int size) {
        heapSize = size;
        //heap = new Type[heapSize];
        //heap = (Type*) new char[heapSize * sizeof(Type)];
        heap = (Type*) std::malloc(heapSize * sizeof(Type));
        //can make void* for faster allocation
        
        for (int i = 0; i < heapSize; i++)
            availableIndexes.push(i);
#ifdef TEST_DATA
        //test, everything depends on this calculation
        for (int i = 0; i < heapSize; i++) {
            //assert(&heap[i] == (heap + i * sizeof(Type)));
            assert(&heap[i] == (heap + i)); //sizeof done for me :)
        }
#endif
    }
    
public:
    Heap() {
        init(DEFAULT_SIZE);
    }
    
    Heap(int initSize) {
        init(initSize);
    }
    
    ~Heap() {
        //delete [] heap;
        std::free((void*)heap);
        //delete [] (char*)heap;
    }
    
    Type* malloc(const Type& constructor, int expectedIndex = -1) {
        if (availableIndexes.size() == 0) {
            throw "Out of my Heap Memory";
        }
        else {
            int index = availableIndexes.front();
            availableIndexes.pop();
            
            heap[index] = constructor; //extra step
#ifdef TEST_DATA
            assert(expectedIndex < 0 || index == expectedIndex);
#endif
            return &heap[index];
        }
    }
    
    Type* malloc(int expectedIndex = -1) {
        return malloc(Type(), expectedIndex);
    }
    
    //sizeof is done for me :)
    //address = startingAddress + offset
    void free(Type* garbage, int expectedIndex = -1) {
        //unsigned long index = (garbage - heap) / sizeof(Type);
        long index = garbage - heap;
        availableIndexes.push((int)index);
#ifdef TEST_DATA
        assert(expectedIndex < 0 || index == expectedIndex);
#endif
        //cout << "Index " << index << " is now free" << endl;
    }
};

#endif /* defined(__Heap__Heap__) */
