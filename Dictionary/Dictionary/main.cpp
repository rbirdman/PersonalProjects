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

#include "Dictionary.h"

using namespace std;

void testWordLevel();

void testDictionary();
void testInsertAndContains();
void testPrefix();
void testLowerBound();

int main(int argc, const char * argv[]) {
//	testWordLevel();
	testDictionary();
	
	// insert code here...
	cout << "Passed all tests" << endl;
	std::cout << "Hello, World!\n";
	
	return 0;
}

void testWordLevel() {
	WordLevel level;
	
	level.insert("");
	level.insert("Bill");
	level.insert("Adam");
	level.insert("Dave");
	level.insert("Xander");
	
	assert(!level.contains(""));
	assert(level.contains("Bill"));
	assert(level.contains("Adam"));
	assert(level.contains("Dave"));
	assert(level.contains("Xander"));
	
	assert(!level.contains(""));
	assert(!level.contains("John"));
	assert(!level.contains("Fred"));
	assert(!level.contains("bill"));
	assert(!level.contains("Kyle"));
	assert(!level.contains("B"));
	assert(!level.contains("Ad"));
	assert(!level.contains("Dav"));
	assert(!level.contains("Xander."));
}

void testDictionary() {
	testInsertAndContains();
	testPrefix();
//	testLowerBound();
}

void testSet() {
	const char* file = "dictionary.txt";
	
	set<string> list;
	string word;
	
	ifstream loadFile(file);
	
	//int count = 0;
	
	while (loadFile >> word) {
	//		count++;
		list.insert(word);
	}
	
	ifstream readFile(file);
	
	//	cout << "Measured size: " << count << endl;
	//	cout << "size: " << list.size() << endl;
	cout << file << " was" << (readFile.is_open()?"" : " not") << " opened." << endl;
	
	while(readFile >> word)
		list.find(word);
}

void testTrie() {
	const char* file = "dictionary.txt";
	
	Dictionary dict(file);	
	string word;
	
	ifstream readFile(file);
	
	cout << file << " was" << (readFile.is_open()?"" : " not") << " opened." << endl;
	
	while (readFile >> word)
		assert(dict.containsWord(word));
}

void testInsertAndContains() {
	//testSet();
	testTrie();
}

void testPrefix() {
	Dictionary level;
	
	level.insertWord("");
	level.insertWord("Bill");
	level.insertWord("Adam");
	level.insertWord("Dave");
	level.insertWord("Xander");
	
	assert(level.isPrefix("Bill"));
	assert(level.isPrefix("Adam"));
	assert(level.isPrefix("Dave"));
	assert(level.isPrefix("Xander"));
	
	assert(level.isPrefix(""));
	assert(!level.isPrefix("John"));
	assert(!level.isPrefix("Fred"));
//	assert(!level.isPrefix("bill"));
	assert(!level.isPrefix("Kyle"));
	assert(level.isPrefix("B"));
	assert(level.isPrefix("Ad"));
	assert(level.isPrefix("Dav"));
	assert(!level.isPrefix("Xander."));
}

void testLowerBound() {
	Dictionary dict;
	
	dict.insertWord("apple");
	dict.insertWord("orange");
	dict.insertWord("banana");
	dict.insertWord("grape");
	
	string val;
	assert(dict.contains("apple"));
	val = dict.findClosestTo("apple");
	cout << val << endl;
	assert(val == "apple");
	val = dict.findClosestTo("app");
	assert(val == "apple");
	val = dict.findClosestTo("apples");
	assert(val == "banana");
	val = dict.findClosestTo("bana");
	assert(val == "banana");
	val = dict.findClosestTo("banana");
	assert(val == "banana");
	val = dict.findClosestTo("bananas");
	assert(val == "grape");
	val = dict.findClosestTo("grape");
	assert(val == "grape");
	val = dict.findClosestTo("orange");
	assert(val == "orange");
	val = dict.findClosestTo("oranges");
	assert(val == "");
}
