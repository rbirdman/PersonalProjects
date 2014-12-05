//
//  Heap.cpp
//  Heap
//
//  Created by Ryan Bird on 11/11/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include "Heap.h"
/*
#define DEFAULT_SIZE 20

//template <typename Type>
template <class Type>
Heap<Type>::Heap() {
    init(DEFAULT_SIZE);
}

template <typename Type>
Heap<Type>::Heap(int initSize) {
    init(initSize);
}

template <typename Type>
void Heap<Type>::init(int size) {
    heapSize = size;
    heap = new Type[heapSize];
    //can make void* for faster allocation
    
    for (int i = 0; i < heapSize; i++)
        availableIndexes.push(i);
    
    //test, everything depends on this calculation
    for (int i = 0; i < heapSize; i++) {
        assert(&heap[i] == (heap + i * sizeof(Type)));
    }
}

template <typename Type>
Heap<Type>::~Heap() {
    delete [] heap;
}

//address = startAddress + offset * sizeof(Type)
template <typename Type>
Type* Heap<Type>::malloc() {
    if (availableIndexes.size() == 0) {
        throw "Out of my Heap Memory";
    }
    else {
        int index = availableIndexes.front();
        availableIndexes.pop();
        
        heap[index] = Type(); //extra step
        return &heap[index];
    }
}

//address = startAddress + offset * sizeof(Type)
template <typename Type>
void Heap<Type>::free(Type* garbage) {
    int index = (garbage - heap) / sizeof(Type);
    availableIndexes.push(index);
    
    cout << "Index " << index << " is now free" << endl;
}
*/
