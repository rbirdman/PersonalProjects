//
//  main.cpp
//  Heap
//
//  Created by Ryan Bird on 11/11/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include <iostream>
#include <string>


#define TEST_DATA

#include "Heap.h"
#include "AdvancedHeap.h"
#include "BorrowHeap.h"

using namespace std;

//#define HEAP_SIZE 20
#define HEAP_SIZE 5000

void testHeap();
void testAdvancedHeap();


void testBorrowHeap();
void testBorrow();
void testSizes();
void testLarge();
void testMerge();

void testNull(BorrowHeap& heap, int allocSize);
void testNotNull(BorrowHeap& heap, int allocSize);

int main(int argc, const char * argv[]) {
    testHeap();
    testAdvancedHeap();
    testBorrowHeap();
    
    return 0;
}

//the extra index as a parameter in each call is for testing purposes
void testHeap() {
    cout << "Size void*: " << sizeof(void*) << endl;
    cout << "Size uint64_t*: " << sizeof(uint64_t*) << endl;
    
    Heap<unsigned int> heap(HEAP_SIZE);
    
    unsigned int* data[HEAP_SIZE];
    
    for (int i = 0; i < HEAP_SIZE; i++)
        data[i] = heap.malloc(i);
    
    for (int i = 0; i < HEAP_SIZE; i++)
        heap.free(data[i], i);
    
    try {
        for (int i = 0; i < HEAP_SIZE + 1; i++)
            data[i] = heap.malloc(i);
        assert(false); //should've thrown out of memory exception
        
    } catch (string exception) {
        cout << "Caught: " << exception << endl;
    } catch(const char* exception) {
        cout << "Caught char*: " << exception << endl;
    }
    
    //full heap here
    heap.free(data[17], 17);
    //manually check that index 17 is malloc'd
    data[17] = heap.malloc(17);
    
    for (int i = 0; i < HEAP_SIZE; i++)
        heap.free(data[i], i);
    
    //this should still malloc each individual index 0 -> 19
    for (int i = 0; i < HEAP_SIZE; i++)
        heap.free(heap.malloc(i), i);
}

void testAdvancedHeap() {
    AdvancedHeap heap;
    //MAX size for now: 20 indexes per row of allocations
    //5 rows
    void* data[101]; //last should be null
    
    //100 mallocs == full, 101 returns null value
    for (int i = 0; i < 101; i++) {
        int expected = 8 << i / 20;
        data[i] = heap.malloc(8, (expected > 128)?0:expected);
    }
    
    //TEST: each address is unique
    for (int i = 0; i < 100; i++) {
        assert(data[i] != NULL);
        //assert(data[i] != data[i+1]);
        for (int index = i + 1; index < 100; index++) {
            assert(data[i] != data[index]);
        }
        //unique(data[i], data[100], data[i]);
    }
    
    //TEST: spacing of each address
    int size = 8;
    for (int i = 0; i < 99; i++) {
        if ((i + 1) % 20 == 0) {
            size <<= 1;
            continue;
        }
        
        char* first = (char*) data[i];
        char* second = (char*) data[i+1];
        
        long value = second - first;
        
        assert(value == size);
    }
    
    //last allocation was a full heap, returns NULL
    assert(data[100] == NULL);
    
    //full heap
    for (int i = 0; i <= 200; i++) {
        assert(heap.malloc(i,0) == NULL);
    }
    
    for (int i = 0; i < 100; i++) {
        heap.free(data[i], MIN_ALLOC_SIZE << i / 20);
    }
    
    //test that malloc and free acquires correct blocksize
    //depending on request
    heap.free(heap.malloc(7, 8), 8);
    heap.free(heap.malloc(8, 8), 8);
    heap.free(heap.malloc(9, 16), 16);
    
    heap.free(heap.malloc(15, 16), 16);
    heap.free(heap.malloc(16, 16), 16);
    heap.free(heap.malloc(17, 32), 32);
    
    heap.free(heap.malloc(31, 32), 32);
    heap.free(heap.malloc(32, 32), 32);
    heap.free(heap.malloc(33, 64), 64);
    
    heap.free(heap.malloc(63, 64), 64);
    heap.free(heap.malloc(64, 64), 64);
    heap.free(heap.malloc(65, 128), 128);
    
    heap.free(heap.malloc(127, 128), 128);
    heap.free(heap.malloc(128, 128), 128);
    heap.free(heap.malloc(129, 0), 0);
    
    for (int i = 0; i < 200; i++) {
        int expected = MIN_ALLOC_SIZE;
        while (expected < i)
            expected <<= 1;
        expected = (expected > 128)?0:expected;
        heap.free(heap.malloc(i, expected), expected);
    }
    //heap = AdvancedHeap();
    
}

void testBorrowHeap() {
    testBorrow();
    testSizes();
    testLarge();
    testMerge();
}

void testBorrow() {
    BorrowHeap heap(64);
    void* addresses[8];
    
    for (int i = 0; i < 8; i++) {
        addresses[i] = heap.malloc(8);
    }
    
    for (int i = 0; i < 8; i++) {
        for (int c = i + 1; c < 8; c++) {
            char* first = (char*) addresses[i];
            char* second = (char*) addresses[c];
            
            assert(first != second);
            assert(second - first == (c - i) * 8);
        }
    }
    
    testNull(heap, 8);
    
    for (int i = 0; i < 8; i++) {
        heap.free(addresses[i]);
    }
}

void testSizes() {
    BorrowHeap heap(64);
    //32 16 8 8
    
    void* addresses[4];
    addresses[0] = heap.malloc(8);
    addresses[3] = heap.malloc(32);
    addresses[2] = heap.malloc(16);
    addresses[1] = heap.malloc(8);
    
    assert((char*) addresses[1] - (char*) addresses[0] == 8);
    assert((char*) addresses[2] - (char*) addresses[1] == 8);
    assert((char*) addresses[3] - (char*) addresses[2] == 16);
    
    for (int i = 0; i < 4; i++)
        heap.free(addresses[i]);
}

#define largeSize 8160
#define bufferSize largeSize/8

void testLarge() {
    BorrowHeap heap(largeSize);
    
    void* buffer[bufferSize];
    set<void*> sorted;
    
    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = heap.malloc(8);
        assert(buffer[i] != NULL);
        
        //it apparently doesn't come in complete order, but comes eventually
        //...interesting
        sorted.insert(buffer[i]);
    }
    testNull(heap, 8);
    
    void* previous = NULL;
    for (set<void*>::iterator iter = sorted.begin(); iter != sorted.end(); iter++) {
        void* curr = *iter;
        if(previous != NULL) {
            assert((char*)curr - (char*)previous == 8);
        }
        previous = curr;
    }
}

#define smallSize 128
#define smallBuffer smallSize / 8

void testMerge() {
    BorrowHeap heap(128);
    void* buffer[smallBuffer];
    
    for (int i = 0; i < smallBuffer; i++) {
        buffer[i] = heap.malloc(8);
        assert(buffer[i] != NULL);
    }
    
    testNull(heap, 8);
    testNull(heap, 16);
    testNull(heap, 32);
    testNull(heap, 64);
    testNull(heap, 128);
    int freeBytes = 0;
    
    for (int i = 0; i < smallBuffer; i++) {
        if (freeBytes >= 128)
            testNotNull(heap, 128);
        else
            testNull(heap, 128);
        
        if (freeBytes >= 64)
            testNotNull(heap, 64);
        else
            testNull(heap, 64);
        
        if (freeBytes >= 32)
            testNotNull(heap, 32);
        else
            testNull(heap, 32);
        
        if (freeBytes >= 8)
            testNotNull(heap, 8);
        else
            testNull(heap, 8);
        
        heap.free(buffer[i]);
        freeBytes += 8;
    }
    
    testNotNull(heap, 128);
}

void testNull(BorrowHeap& heap, int allocSize) {
    void* ptr = heap.malloc(allocSize);
    assert(ptr == NULL);
}
void testNotNull(BorrowHeap& heap, int allocSize) {
    void* ptr = heap.malloc(allocSize);
    assert(ptr != NULL);
    heap.free(ptr);
}
