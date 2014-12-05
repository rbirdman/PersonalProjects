//
//  Node.h
//  Dictionary
//
//  Created by Ryan Bird on 2/26/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//

#ifndef __Dictionary__Node__
#define __Dictionary__Node__

#include <iostream>
#include <map>

#include <assert.h>

using namespace std;

//26 letters + 0 grammar spots, grammar causes issues
#define NUM_ALPHA 26

//	one byte of data (info var)
//	WSSS SSSS
//	W: is word bit			x80		1000 0000
//	S: size of data array	x7F		0111 1111
#define IS_WORD_BIT 0x80
#define SIZE_BITS 0x7F

class LeafNode;

class Node {
private:
	
	
protected:
	
	inline unsigned char size();
	inline bool isWord();
	
	int info;	//one byte of data, stores size & word bit
	
	void incrementSize();
	void setAsWord();
	
	static LeafNode End;
	
public:
	Node();
	virtual ~Node();
	
	static Node* EndNode();
	
	//originally protected
	virtual bool search(const char* word) = 0;
	virtual Node* add(const char*) = 0;
	virtual Node* removeWord(const char*) = 0;
	virtual Node* access(char c) = 0;
	
	inline void setStatus(unsigned char);
	Node* insert(const char* word);
	bool remove(const char* word);
	
	bool contains(const char* word);
	virtual bool isPrefix(const char* word);
};

class LeafNode: public Node {
public:
	bool search(const char* word);
	Node* add(const char*);
	Node* removeWord(const char*);
	Node* access(char c);
	
	LeafNode();
	~LeafNode();
	
	bool contains(const char* word);
	bool remove(const char* word);
};

class FilledNode;

class SmallNode : public Node {
	map<char, Node*> data;
	
	FilledNode* convertToFilledNode();
public:
	bool search(const char* word);
	Node* add(const char*);
	Node* removeWord(const char*);
	Node* access(char c);
	
	SmallNode();
	~SmallNode();
	
	bool contains(const char* word);
	bool remove(const char* word);
};

class FilledNode : public Node {
	
	void resize();
	
	//MemoryWordLevel* data[]; //pointer to array of pointers
	Node** data;
	inline int index(char);

public:
	bool search(const char* word);
	Node* add(const char*);
	void add(char c, Node*);
	
	Node* removeWord(const char*);
	Node* access(char c);


	FilledNode();
	~FilledNode();
	
	bool contains(const char* word);
	bool remove(const char* word);
};


#endif /* defined(__Dictionary__Node__) */
