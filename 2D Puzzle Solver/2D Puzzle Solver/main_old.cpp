//
//  main.cpp
//  2D Puzzle Solver
//
//  Created by Ryan Bird on 7/6/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

#define SHUFFLE_TIMES 1
//#define ROWS 4
//#define COLS 4
#define ROWS 3
#define COLS 3

#define LINEAR_LENGTH ROWS * COLS
#define MAX_DISTANCE (ROWS - 1) + (COLS - 1)
#define MAX_HEURISTIC MAX_DISTANCE * LINEAR_LENGTH
#define COORD(row,col) row * COLS + col

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <stack>

//coord[1] = {0,0}; number 1 needs to end up at 0,0
//const uint8_t coordinates[LINEAR_LENGTH][2] = {
//	{0xFF,0xFF}, {0,0}, {0,1}, {0,2},
//	{0,3}, {1,0}, {1,1}, {1,2},
//	{1,3}, {2,0}, {2,1}, {2,2},
//	{2,3}, {3,0}, {3,1}, {3,2}
//};
const uint8_t coordinates[LINEAR_LENGTH][2] = {
	{0xFF,0xFF},
	{0,0}, {0,1}, {0,2},
	{1,0}, {1,1}, {1,2},
	{2,0}, {2,1},
};


enum Direction {
	UP, DOWN, LEFT, RIGHT, NO_DIRECTION
};

using namespace std;

void printBoard(uint8_t numbers[LINEAR_LENGTH]);
uint64_t heuristic(uint8_t numbers[LINEAR_LENGTH]);
void solve(uint8_t numbers[LINEAR_LENGTH]);

int main(int argc, const char * argv[]) {
	uint8_t numbers[LINEAR_LENGTH];
//	uint8_t numbers[LINEAR_LENGTH] = {
//		0, 12, 1, 5,
//		15, 6, 10, 9,
//		2, 7, 11, 13,
//		3, 4, 8 , 14
//	};
	
	for (int i = 0; i < LINEAR_LENGTH; i++)
		numbers[i] = i;
	
	for (int count = 0; count < SHUFFLE_TIMES; count++) {
		random_shuffle(numbers, numbers + LINEAR_LENGTH);
	}
	
	printBoard(numbers);
	printf("Total Value: %d\n", heuristic(numbers));
	solve(numbers);
	
	return 0;
}

void printBoard(uint8_t numbers[LINEAR_LENGTH]) {
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			uint8_t value = numbers[COORD(row, col)];
			if (value == 0)
				printf("   ");
			else
				printf(" %2d", value);
			
		}
		printf("\n");
	}
}

//furthest individual distance is 6: (ROWS - 1) + (COLS - 1)
uint64_t heuristic(uint8_t numbers[LINEAR_LENGTH]) {
	uint64_t value = 0;
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			int curr_num = numbers[COORD(row, col)];
			if (curr_num != 0) {
				const uint8_t* coords = coordinates[curr_num];
				
				int distance = abs(coords[0] - row) + abs(coords[1] - col);
				//make lower numbers less costlier (give them priority to be moved)
//				distance += curr_num;
				distance *= curr_num + 1;
				
				
//				printf("%d: %d\n", curr_num, distance);
				value += distance;
			}
			
		}
	}
	
	//std::priority_queue is MAX priority queue
	return value; //heuristic is a measurement of cost
//	return MAX_HEURISTIC - value;
}

uint64_t uniqueID(uint8_t numbers[LINEAR_LENGTH]) {
	uint64_t id = 0;
	
	for (int i = 0; i < LINEAR_LENGTH; i++) {
		id <<= 4;
		id += numbers[i];
	}
	
	return id;
}

struct Node {
	uint8_t board[16];
//	uint8_t empty_loc[2];
	Direction move; //move that got you to this point    //move to make with this setup
	Node* parent;
	uint64_t cost; //heuristic + parent->cost
	
	Node() : cost(0), parent(NULL), move(NO_DIRECTION) {
	}
};

bool operator<(const Node& node1, const Node& node2) {
	return node1.cost < node2.cost; //may need to switch these
}

struct comparePointer{
	bool operator()(const Node* node1, const Node* node2) {
		return node2->cost < node1->cost;
	}
};

Node* makeMove(Node* node, Direction dir, int row, int col) {
	Node* child = new Node();
	child->parent = node;
	child->move = dir;
	
	memcpy(child->board, node->board, sizeof(uint8_t) * LINEAR_LENGTH);
	int rowFrom = row, colFrom = col;
	
	switch (dir) {
		case UP:
			rowFrom += 1;
			break;
		case DOWN:
			rowFrom -= 1;
			break;
		case LEFT:
			colFrom += 1;
			break;
		case RIGHT:
			colFrom -= 1;
			break;
		default:
			break;
	}
	
	//make move
	child->board[COORD(row,col)] = child->board[COORD(rowFrom, colFrom)];
	//source is now empty
	child->board[COORD(rowFrom, colFrom)] = 0;
//	printBoard(child->board);
	
	uint64_t distanceLeft = heuristic(child->board);
	
	int64_t cost = node->cost - distanceLeft;
	if (distanceLeft == 0)
		cost = 0;
	else
		cost = (node->cost - cost) + 1;
	
	
	if (child->cost == node->cost) {
		puts("Ending");
	}
	
	child->cost = node->cost + cost;
	
//	child->cost = node->cost + heuristic(child->board);
//	child->cost = heuristic(child->board);
	
	
	
	return child;
}

#define directionAction(direction)\
	Node* child = makeMove(curr, direction, row, col);\
	uint64_t id = uniqueID(child->board);\
	if(ids.count(id)) {\
		delete child;\
	}\
	else {\
		ids.insert(id);\
		nodes.push_back(child);\
		node_queue.push(child);\
		if (child->cost == curr->cost) {\
			curr = child;\
			break;\
		}\
	}


void solve(uint8_t numbers[LINEAR_LENGTH]) {
	vector<Node*> nodes;
//	priority_queue<Node*, vector<Node*>, std::greater<Node*> > node_queue; //priority, minimal cost
	priority_queue<Node*, vector<Node*>, comparePointer > node_queue;
	set<uint64_t> ids;
	
	Node* curr = new Node();
	curr->cost = heuristic(numbers);
	memcpy(curr->board, numbers, sizeof(uint8_t) * LINEAR_LENGTH);
	node_queue.push(curr);
	
	while (node_queue.size()) {
		curr = node_queue.top();
		node_queue.pop();
		
		uint8_t* curr_numbers = curr->board;
//		if (curr->cost) {
//			printf("Current board\n");
//			printBoard(curr_numbers);
//			printf("\n");
//		}
		
		//locate empty space
		int row, col;
		bool found = false;
		for (row = 0; row < ROWS; row++) {
			for (col = 0; col < COLS; col++) {
				if (curr_numbers[COORD(row, col)] == 0) {
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
//		if (curr->cost) {
//			printf("Zero located at: (%d,%d)\n", row, col);
//		}
		
		
		//move in each direction, if child cost == parent cost, found destination
		if (0 < row && curr->move != UP) {
			//slide top piece down
			directionAction(DOWN);
		}
		if (0 < col && curr->move != LEFT) {
			//slide left piece right
			directionAction(RIGHT);
		}
		if (row < ROWS - 1 && curr->move != DOWN) {
			//slide bottom piece up
			directionAction(UP);
		}
		if (col < COLS - 1 && curr->move != RIGHT) {
			//slide right piece left
			directionAction(LEFT);
		}
		
		if (node_queue.size() == 0) {
			puts("Out of nodes");
		}
	}
	
	puts("");
	
	//curr is at finishing point
	stack<Node*> solution;
	int moves = 0;
	while (curr != NULL) {
		solution.push(curr);
		curr = curr->parent;
		moves++;
	}
	
	puts("Solution");
	while (solution.size()) {
		Node* current = solution.top();
		solution.pop();
		
		printBoard(current->board);
		printf("Cost: %d\n", (int) current->cost);
		printf("\n");
		curr = current;
	}
	
	puts("FINISHED");
	printBoard(curr->board);
	
	printf("%d nodes explored\n", (int) nodes.size());
	printf("%d moves for solution\n", moves);
	printf("Cost of %d\n", (int) curr->cost);
	
//	11420 nodes explored
//	26 moves for solution
//	Cost of 179
	
//	New Cost function
//	10405 nodes explored
//	26 moves for solution
//	Cost of 1021
	
	//garbage collect
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
}
