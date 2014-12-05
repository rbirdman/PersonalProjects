//
//  Dictionary.h
//  Dictionary
//
//  Created by Ryan Bird on 2/15/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#ifndef __Dictionary__Dictionary__
#define __Dictionary__Dictionary__

#include <iostream>
#include <fstream>

#include <cstdio>
#include <string>
#include <map>

#include "Node.h"
#include <assert.h>

using namespace std;

class WordLevel : public map<char, WordLevel> {
	bool search(const char* word);
	void findLowest(const char*, string& save);
	
	bool isWord;
public:
	
	WordLevel();
	
	void lower_bound(const char*, string& save);
	void insert(const char* word);
	bool contains(const char* word);
	bool remove(const char* word);
	bool isPrefix(const char* word);
};

//26 letters + 1 grammar spot
#define NUM_ALPHA 27

class FastWordLevel {
	bool search(const char* word);
	
	FastWordLevel* data[NUM_ALPHA];
	bool isWord;
	int index(char);
	
	inline FastWordLevel* & access(char c);
public:
	
	FastWordLevel();
	~FastWordLevel();
	
	void insert(const char* word);
	bool contains(const char* word);
	bool isPrefix(const char* word);
};

//	one byte of data (info var)
//	WSSS SSSS
//	W: is word bit			x80		1000 0000
//	S: size of data array	x7F		0111 1111

#define IS_WORD_BIT 0x80
#define SIZE_BITS 0x7F
class MemoryWordLevel {
	bool search(const char* word);
	
	void resize();
	
	//MemoryWordLevel* data[]; //pointer to array of pointers
	MemoryWordLevel** data;
	char info;	//one byte of data, stores size & word bit
	int index(char);
	inline MemoryWordLevel* & access(char c);
public:
	
	MemoryWordLevel();
	~MemoryWordLevel();
	
	inline unsigned char size();
	
	void insert(const char* word);
	bool contains(const char* word);
	bool isPrefix(const char* word);
};

class Dictionary {
//	FastWordLevel data;
	Node * root;
	
public:
	Dictionary();
    Dictionary(const char* fileName);
	~Dictionary();
	
	void insertWord(const string&);
	void removeWord(const string&);
	void removeLastWord();
	void loadFile(const char*);
	bool contains(const char*);
	void printDictionary();
	string findClosestTo(const string& val);
	
    bool containsWord(const string&);
	bool isPrefix(const string&);
};



#endif /* defined(__Dictionary__Dictionary__) */
