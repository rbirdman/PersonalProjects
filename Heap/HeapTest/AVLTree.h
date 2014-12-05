//
//  AVLTree.h
//  lab6-AVLTree
//
//  Created by Ryan Bird on 3/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef lab6_AVLTree_AVLTree_h
#define lab6_AVLTree_AVLTree_h

#include <algorithm>

#include "MyQueue.h"
#include "Heap.h"

#define USE_MY_HEAP

template<typename E> class AVLNode;

using namespace std;

template <typename T>
class AVLTree {
#ifdef USE_MY_HEAP
    static Heap<AVLNode<T> > myHeap;
#endif
	AVLNode<T> * root;
	int size;
	
	void clearNode(AVLNode<T> * node) {
		if (node != NULL) {
			clearNode(node->left);
			clearNode(node->right);
#ifdef USE_MY_HEAP
            myHeap.free(node);
#else
			delete node;
#endif
		}
	}
	
	AVLNode<T> * insert(AVLNode<T> * node, const T & item) {
		if (node == NULL) {
			size++;
#ifdef USE_MY_HEAP
            return myHeap.malloc(AVLNode<T>(item));
#else
			return new AVLNode<T>(item);
#endif
		}
		else if(node->value < item)
			node->right = insert(node->right, item);
		else if(item < node->value)
			node->left = insert(node->left, item);
		
		node->calculateHeight();
		return node->balance();
	}
	
	AVLNode<T>* getSuccessor(AVLNode<T>* node) {
		AVLNode<T>* successor = NULL;
		
		if (node->left == NULL)
			successor = node->right;
		else if(node->right == NULL)
			successor = node->left;
		else {
			node->right = removeSmallestValue(node->right, successor);
			
			successor->left = node->left;
			if(node->right != successor)
				successor->right = node->right;
		}
		
		if(successor != NULL) {
			successor->calculateHeight();
			successor = successor->balance();
		}
		return successor;
	}
	
	AVLNode<T> * remove(AVLNode<T> * node, const T & item, T & toReturn) {
		if (node == NULL)
			return NULL;
		else if(node->value == item) {
			toReturn = node->value;
			AVLNode<T> * successor = getSuccessor(node);
#ifdef USE_MY_HEAP
            myHeap.free(node);
#else
			delete node;
#endif
			size--;
			return successor;
		}
		else if(node->value < item)
			node->right = remove(node->right, item, toReturn);
		else
			node->left = remove(node->left, item, toReturn);
		
		node->calculateHeight();
		return node->balance();
	}
	
	bool contains(AVLNode<T> * node, T & item) {
		if (node != NULL) {
			if (node->value == item)
				return true;
			else if(node->value < item)
				return contains(node->right, item);
			else
				return contains(node->left, item);
		}
		return false;
	}
	
	AVLNode<T> * findSmallestValue(AVLNode<T> * node) {
		if (node != NULL) {
			while (node->left != NULL)
				node = node->left;
		}
		return node;
	}
	
	AVLNode<T> * removeSmallestValue(AVLNode<T> * node, AVLNode<T> * &toReturn) {
		if (node != NULL) {
			if (node->left != NULL)
				node->left = removeSmallestValue(node->left, toReturn);
			else {
				toReturn = node;
				return node->right;
			}
		}
		
		node->calculateHeight();
		return node->balance();
//		return node;
	}
	
	void printLevel(Queue<AVLNode<T> *>& nodes, int level, ostream & output) {
		Queue<AVLNode<T> *> children;
		
		for(int count = 0; !nodes.empty(); count++) {
			AVLNode<T> * curr = nodes.pop();
			if(count % 8 == 0)
				output << endl << "level " << level << ":";
			
			if (curr->left != NULL)
				children.add(curr->left);
			if (curr->right != NULL)
				children.add(curr->right);
			output << " " << *curr;
		}
		
		if (!children.empty())
			printLevel(children, level + 1, output);
	}
	
public:
	AVLTree<T>() : root(NULL), size(0) {
	}
	
	const AVLNode<T> * getRoot() const {
		return root;
	}
	
	AVLNode<T> * getNodeWithValue(const T & item) {
		AVLNode<T> * curr = root;
		while (curr != NULL && !(curr->value == item)) {
			if (curr->value < item)
				curr = curr->right;
			else
				curr = curr->left;
		}
		return curr;
	}
	
	int getSize() {
		return size;
	}
	
	~AVLTree<T>() {
		clear();
	}
	
	void clear() {
		clearNode(root);
		root = NULL;
		size = 0;
	}
	
	void insert(const T & item) {
		root = insert(root, item);
	}
	
	T remove(const T & item) {
		T toReturn;
		root = remove(root, item, toReturn);
		return toReturn;
	}
	
	bool contains(T & item) {
		return contains(root, item);
	}
	
	void print(ostream & output) {
		if (root != NULL) {
			Queue<AVLNode<T> *> rootLevel;
			if(root != NULL)
				rootLevel.add(root);
			printLevel(rootLevel, 0, output);
		}
	}
};

template <typename E>
class AVLNode {
	friend class AVLTree<E>;
	AVLNode * left;
	AVLNode * right;
	
	int height;
	E value;
	
	static int getHeight(AVLNode * node) {
		if (node == NULL)
			return 0;
		else
			return node->height;
	}
	
	int balanceFactor() {
		return getHeight(left) - getHeight(right);
	}
	
	AVLNode<E> * leftRotation(AVLNode<E> * node) {
		AVLNode<E> * successor = node->right;
		node->right = successor->left;
		successor->left = node;
		
		node->calculateHeight();
		successor->calculateHeight();
		return successor;
	}
	
	AVLNode<E> * rightRotation(AVLNode<E> * node) {
		AVLNode<E> * successor = node->left;
		node->left = successor->right;
		successor->right = node;
		
		node->calculateHeight();
		successor->calculateHeight();
		return successor;
	}
	
public:
#ifdef USE_MY_HEAP
//    AVLNode() : left(NULL), right(NULL), height(1) {
//    }
#endif
    
	AVLNode(const E & val) : left(NULL), right(NULL), height(1), value(val) {
	}
	
	void calculateHeight() {
		height = max(getHeight(left), getHeight(right)) + 1;
	}
	
	void print(ostream & output) {
		output << value << " (" << height << ")";
	}
	
	AVLNode<E> * balance() {
		int result = balanceFactor();
		if (-1 <= result && result <= 1)
			return this;
		else if (result > 0) { // left height > right height
			if (left->balanceFactor() < 0)
				left = leftRotation(left);
			return rightRotation(this);
		}
		else { //result < 0, right height > left height
			if (right->balanceFactor() > 0)
				right = rightRotation(right);
			return leftRotation(this);
		}
	}
	
	const AVLNode<E> * getLeft() const {
		return left;
	}
	
	const AVLNode<E> * getRight() const {
		return right;
	}
	
	const E & getValue() const {
		return value;
	}
	
	int getHeight() const {
		return height;
	}
	
	void printTree(ostream& output) {
		Queue<AVLNode<E> *> rootLevel;
		rootLevel.add(this);
		printLevel(rootLevel, this->height, output);
	}
	
	void printLevel(Queue<AVLNode<E> *>& level, int height, ostream& output) {
		if (height > 0) {
			Queue<AVLNode<E>*> children;
			
			while (!level.empty()) {
				AVLNode<E>* node = level.pop();
				
				for (int i = 0; i < height; i++)
					output << '\t';
				
				if (node != NULL) {
					//output << node->value;
					node->print(output);
					children.add(node->left);
					children.add(node->right);
				}
				else {
					output << '\t';
					children.add(NULL);
					children.add(NULL);
				}
			}
			output << '\n';
			printLevel(children, height - 1, output);
		}
	}
};

#ifdef USE_MY_HEAP
template <typename T>
Heap<AVLNode<T> > AVLTree<T>::myHeap(300250);
#endif

#endif

template <typename E>
ostream & operator<<(ostream & output, AVLNode<E> & node) {
	output << node.getValue() << "(" << node.getHeight() << ")";
	
	return output;
}
