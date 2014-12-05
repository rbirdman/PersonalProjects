//
//  Node.cpp
//  Dictionary
//
//  Created by Ryan Bird on 2/26/13.
//  Copyright (c) 2013 Ryan Bird. All rights reserved.
//


// TODO:
// Labyrinth idea, Start node & End node, all words end at end node (Ben's idea)
// EntryNode: We want to have a flexible array in order to be able to hash to an index and still be fair with memory. The current memory node can't rehash to a bigger size since it doesn't know the exact value that cause it to hash there.

//  Entry Node will have an array of entries where it has a Key and value pair. The Key will be a char and value will be a Node*. This will allow for rehashing and allow for punctuation to be stored in the dictionary

#include "Node.h"

LeafNode Node::End;

Node* Node::EndNode() {
	return &Node::End;
}

Node::Node() : info(0) {
	isWord(); // checks to make sure they are equal
}

Node::~Node() {
	
}

unsigned char Node::size() {
	return info & SIZE_BITS;
}

bool Node::isWord() {
	return info & IS_WORD_BIT;
}

void Node::incrementSize() {
//	info = (info & IS_WORD_BIT) | (size() + 1);
	info++;
}

void Node::setAsWord() {
	info |= IS_WORD_BIT;
}

void Node::setStatus(unsigned char c) {
	info = c;
}

Node* Node::insert(const char* word) {
	if(*word)
		return add(word);
	return this;
}

bool Node::remove(const char* word) {
	removeWord(word);
	
	return true;
}

bool Node::contains(const char* word) {
	if(*word)
		return search(word);
	return false;
}

bool Node::isPrefix(const char *word) {
	if (*word) {
		Node* next = access(*word);
		
		if (next != NULL)
			return next->isPrefix(word + 1);
		return false;
	}
	
	return true;
}

//===============================================================
//Leaf Node

LeafNode::LeafNode() {
	info = 0;
}

LeafNode::~LeafNode() {
	
}

bool LeafNode::search(const char* word) {
	return *word == '\0';
}

Node* LeafNode::add(const char* word) {
	if (*word) {
		SmallNode* temp = new SmallNode();
		temp->setStatus(info);
		temp->add(word);
		
		//delete this;
		
		return temp;
	}
	else {
		setAsWord();
	}
	
	return this;
}

Node* LeafNode::removeWord(const char*) {
	return NULL;
}

Node* LeafNode::access(char c) {
	return NULL;
}

//===============================================================
//Small Node

SmallNode::SmallNode() {
	info = 0;
}

SmallNode::~SmallNode() {
	map<char, Node*>::iterator iter = data.begin();
	
	for (;iter != data.end(); iter++) {
		Node* n = iter->second;
		
		if (n != Node::EndNode())
			delete n;
	}
}

bool SmallNode::search(const char* word) {
	if (*word) {
		Node* next = access(*word);
		
		if (next != NULL)
			return next->search(word + 1);
	}
	
	return isWord();
}

// convert if greater than 3 levels deep (size > 8)

//				1
//		2			3
//	4		5	6		7
//convert to filled node at certain point
Node* SmallNode::add(const char* word) {
	if (*word) {
		Node* next = access(*word);
		
		if(next == NULL) {
			int currSize = size();
			
			if(currSize > 7) {
				FilledNode* temp = convertToFilledNode();
				temp->add(word);
				delete this;
				
				return temp;
			}
			else
				data[*word] = next = Node::EndNode();
//				next = data[*word] = new LeafNode();
			
			//combine Wordbit with new size
			incrementSize();
		}
		
		data[*word] = next->add(word + 1);
	}
	else
		setAsWord();
	
	return this;
}

FilledNode* SmallNode::convertToFilledNode() {
	FilledNode* temp = new FilledNode();
	
	map<char, Node*>::iterator iter = data.begin();
	
	for (; iter != data.end(); iter++)
		temp->add(iter->first, iter->second);
	
	data.clear();
	
	temp->setStatus(info);
	
	return temp;
}

Node* SmallNode::removeWord(const char*) {
	return this;
}

Node* SmallNode::access(char c) {
	map<char, Node*>::iterator iter = data.find(c);
	
	if (iter != data.end())
		return iter->second;
	else
		return NULL;
}

//===============================================================
//Filled Node

FilledNode::FilledNode() {
	info = 0;
	data = new Node*[NUM_ALPHA];
	memset(data, 0, sizeof(Node*) * NUM_ALPHA);
}

FilledNode::~FilledNode() {
	for (int i = 0; i < NUM_ALPHA; i++) {
		Node* n = data[i];
		
		if(n && n != Node::EndNode())
			delete n;
	}
	
	delete [] data;
}

bool FilledNode::search(const char* word) {
	if (*word) {
		Node* next = access(*word);
		
		if (next != NULL)
			return next->search(word + 1);
	}
	
	return isWord();
}

Node* FilledNode::add(const char* word) {
	if (*word) {
		Node* next = access(*word);
		
		if (next == NULL) {
			next = data[index(*word)] = new SmallNode();
			incrementSize();
		}
		data[index(*word)] = next->add(word + 1);
	}
	else
		setAsWord();
	
	return this;
}

void FilledNode::add(char c, Node* node) {
	data[index(c)] = node;
	incrementSize();
}

Node* FilledNode::removeWord(const char*) {
	return this;
}

Node* FilledNode::access(char c) {
	return data[index(c)];
}

//current issues if given anything outside 'a' - 'z'
int FilledNode::index(char c) {
//	if (isalpha(c))
	return c - 'a';
/*	else {
		int temp = c % 3; //3 grammar spots
		return NUM_ALPHA - temp;
		//return NUM_ALPHA - 1;
	}*/
}
