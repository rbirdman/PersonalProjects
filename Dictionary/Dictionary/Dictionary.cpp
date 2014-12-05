//
//  Dictionary.cpp
//  Dictionary
//
//  Created by Ryan Bird on 2/15/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#include "Dictionary.h"

WordLevel::WordLevel() : isWord(false) {
	
}

bool WordLevel::search(const char* word) {
	if (*word) {
		map<char,WordLevel>::iterator iter = find(*word);
		if (iter != end())
			return iter->second.search(word + 1);
		else
			return false;
	}
	
	return isWord;
}

void WordLevel::insert(const char* word) {
	if (*word)
		(*this)[*word].insert(word + 1);
	else
		isWord = true;
}

bool WordLevel::contains(const char* word) {
	if (*word)
		return search(word);
	return false;
}

bool WordLevel::isPrefix(const char *word) {
	if (*word) {
		map<char,WordLevel>::iterator iter = find(*word);
		if (iter != end())
			return iter->second.isPrefix(word + 1);
		else
			return false;
	}
	
	return true;
}

void WordLevel::lower_bound(const char* word, string& save) {
	if (*word) {
//		cout << word << endl;
//		char c = *word;
//		cout << c << endl;
		map<char, WordLevel>::iterator iter = find(*word);
		
		if (iter == end())
			iter->second.findLowest(word + 1, save);
		else {
			save += *word;
			iter->second.lower_bound(word + 1, save);
		}
	}
}

void WordLevel::findLowest(const char* word, string& save) {
	if (*word) {
		//problem
	}
}

FastWordLevel::FastWordLevel() : isWord(false) {
	memset(data, 0, NUM_ALPHA * sizeof(FastWordLevel*));
}

FastWordLevel::~FastWordLevel() {
	for (int i = 0; i < NUM_ALPHA; i++)
		if(data[i])
			delete data[i];
}


inline FastWordLevel* & FastWordLevel::access(char c) {
	return data[index(c)];
}
//problem with non-alpha characters
int FastWordLevel::index(char c) {
	if(isalpha(c))
		return tolower(c) - 'a';
	else
		return NUM_ALPHA - 1;
}

bool FastWordLevel::search(const char* word) {
	if (*word) {
		FastWordLevel* & level = access(*word);
		
		if (level)
			return level->search(word + 1);
		else
			return false;
	}
	
	return isWord;
}

void FastWordLevel::insert(const char* word) {
	if (*word) {
		FastWordLevel* & level = access(*word);
		
		if(level == NULL)
			level = new FastWordLevel();
		
		level->insert(word + 1);
	}
	else
		isWord = true;
}

bool FastWordLevel::contains(const char* word) {
	if (*word)
		return search(word);
	return false;
}

bool FastWordLevel::isPrefix(const char *word) {
	if (*word) {
		FastWordLevel* &level = access(*word);
		
		if (level)
			return level->isPrefix(word + 1);
		else
			return false;
	}
	
	return true;
}

MemoryWordLevel::MemoryWordLevel() : info(0), data(NULL) {
}

MemoryWordLevel::~MemoryWordLevel() {
	for (int i = 0; i < size(); i++)
		if(data[i])
			delete data[i];
	if(data)
		delete [] data;
}


inline MemoryWordLevel* & MemoryWordLevel::access(char c) {
	if(size() == 0)
		resize();
	return data[index(c)];
}

unsigned char MemoryWordLevel::size() {
	return info & SIZE_BITS;
}

//currently allows size of 0 or NUM_ALPHA, no medium crap yet
//rehashing could be difficult to impossible, reconsider medium crap
void MemoryWordLevel::resize() {
	unsigned char Size = size();
	unsigned char newSize;
	
	switch (Size) {
		case 0: newSize = NUM_ALPHA; break;
	}
	
	data = new MemoryWordLevel*[newSize];
	memset(data, 0, newSize * sizeof(MemoryWordLevel*));
	info = (info & IS_WORD_BIT) | newSize;
}

int MemoryWordLevel::index(char c) {
//	if(isalpha(c))
		return tolower(c) - 'a';
//	else
//		return NUM_ALPHA - 1;
}

bool MemoryWordLevel::search(const char* word) {
	if (*word) {
		MemoryWordLevel* & level = access(*word);
		
		if (level)
			return level->search(word + 1);
		else
			return false;
	}
	
	return info & IS_WORD_BIT;
}

void MemoryWordLevel::insert(const char* word) {
	if (*word) {
		MemoryWordLevel* & level = access(*word);
		
		if(level == NULL)
			level = new MemoryWordLevel();
		
		level->insert(word + 1);
	}
	else
		info |= IS_WORD_BIT; //activate bit
}

bool MemoryWordLevel::contains(const char* word) {
	if (*word)
		return search(word);
	return false;
}

bool MemoryWordLevel::isPrefix(const char *word) {
	if (*word) {
		MemoryWordLevel* & level = access(*word);
		
		if (level)
			return level->isPrefix(word + 1);
		else
			return false;
	}
	
	return true;
}

Dictionary::Dictionary() {
	root = Node::EndNode();
}

Dictionary::Dictionary(const char* fileName) {
//	root = new LeafNode();
	root = Node::EndNode();
	loadFile(fileName);
}

Dictionary::~Dictionary() {
	delete root;
}

void Dictionary::insertWord(const string& word) {
//	data.insert(word.data());
	root = root->insert(word.data());
}

void Dictionary::removeWord(const string& word) {
	
}

void Dictionary::removeLastWord() {
	
}

void Dictionary::loadFile(const char* filename) {
	//ifstream file(filename);
	FILE* file = fopen(filename, "r"); // read permissions
	char str[100];
	
	while (fscanf(file, "%s", str) != EOF) {
//		cout << "Dictionary.loadFile(): " << str << endl;
		
//		assert(root->contains(str));
		
		
		/*
		if (strcmp(str, "z") == 0) {
			//sleep(5);
			system("sleep 5");
			break;
		}
		else if(strcmp(str, "z") == 0) {
			cout << "MEMORY LEAK CAUSE OF Z" << endl;
		}
		*/
		root = root->insert(str);
	}
	
	fclose(file);
//		data.insert(str);
}

bool Dictionary::contains(const char* word) {
//	return data.contains(word);
	return root->contains(word);
}

void Dictionary::printDictionary() {
	
}

string Dictionary::findClosestTo(const string& val) {
	string foundWord;
	cout << "searching: " << val << endl;
//	data.lower_bound(val.data(), foundWord);
	
	return foundWord;
}

bool Dictionary::containsWord(const string& word) {
//	return data.contains(word.data());
	return root->contains(word.data());
}

bool Dictionary::isPrefix(const string& val) {
//	return data.isPrefix(val.data());
	return root->isPrefix(val.data());
}