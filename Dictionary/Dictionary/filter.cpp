//
//  main.cpp
//  Dictionary
//
//  Created by Ryan Bird on 2/15/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include <iostream>
#include <assert.h>

#include <vector>
#include <set>

#include <fstream>

//#include "Dictionary.h"

using namespace std;

void testWordLevel();

void testDictionary();
void testInsertAndContains();
void testPrefix();
void testLowerBound();

int main(int argc, const char * argv[]) {
	
	ifstream input("dictionary-grammar.txt");
	ofstream output("dictionary.txt");
	
	for (char c = input.get(); input.good(); c = input.get()) {
		if(isalpha(c) || c == '\n')
			output << c;
	}
	
	return 0;
}
