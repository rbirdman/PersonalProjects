//
//  main.cpp
//  HeapTest
//
//  Created by Ryan Bird on 11/12/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include "AVLTree.h"
#include "Heap.h"

using namespace std;

int main(int argc, const char * argv[]) {
    AVLTree<string> data;
    ifstream in_file("dictionary.txt");
    string word;
    
    while (in_file >> word) {
        data.insert(word);
    }
    
    cout << "Items: " << data.getSize() << endl;
    
    data.clear();
    
    return 0;
}

