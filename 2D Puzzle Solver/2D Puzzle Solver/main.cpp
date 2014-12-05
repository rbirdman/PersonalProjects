//
//  main.cpp
//  2D Puzzle Solver
//
//  Created by Ryan Bird on 7/6/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

#define RANDOM_START

//#define A_STAR //does not complete
#define BEAM_SEARCH //averages 4x4 solution ~ 65 moves, BEAM_SIZE == 1500
//default BEST_FIRST_SEARCH //averages ~ 200 moves
#define BEAM_SIZE 1500

#define SIDE_LEN 4
#define ROWS SIDE_LEN
#define COLS SIDE_LEN

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
#if LINEAR_LENGTH == 16
const uint8_t coordinates[LINEAR_LENGTH][2] = {
	{0xFF,0xFF},
	{0,0}, {0,1}, {0,2}, {0,3},
	{1,0}, {1,1}, {1,2}, {1,3},
	{2,0}, {2,1}, {2,2}, {2,3},
	{3,0}, {3,1}, {3,2}
};
#elif LINEAR_LENGTH == 9

// 10405 nodes explored
const uint8_t coordinates[LINEAR_LENGTH][2] = {
	{0xFF,0xFF},
	{0,0}, {0,1}, {0,2},
	{1,0}, {1,1}, {1,2},
	{2,0}, {2,1},
};

// Unsolvable: 181439 nodes explored
#endif

enum Direction {
	UP, DOWN, LEFT, RIGHT, NO_DIRECTION
};

using namespace std;

void printBoard(uint8_t numbers[LINEAR_LENGTH]);
uint64_t heuristic(uint8_t numbers[LINEAR_LENGTH]);
void solve(uint8_t numbers[LINEAR_LENGTH]);
bool solveable(uint8_t numbers[LINEAR_LENGTH]);
uint inversions(uint8_t numbers[LINEAR_LENGTH]);

// random generator function:
int myrandom (int i) { return std::rand()%i;}

int main(int argc, const char * argv[]) {
#ifdef RANDOM_START
	uint8_t numbers[LINEAR_LENGTH];
	
	srand((unsigned int)time(NULL));
	unsigned int seed = rand();
//	831849122
//	832269297
//	832723086
	
//	BEAM_SEARCH
//	295803828  56 moves
	
	printf("Seed: %d\n", seed);
	srand(seed);
	
	for (int i = 0; i < LINEAR_LENGTH; i++)
		numbers[i] = i;
	
	do {
		random_shuffle(numbers, numbers + LINEAR_LENGTH, myrandom);
	} while (!solveable(numbers));
	
#else
	uint8_t numbers[LINEAR_LENGTH] = {
		0, 12, 1, 5,
		15, 6, 10, 9,
		2, 7, 11, 13,
		3, 4, 8 , 14
	};
#endif
	
	printBoard(numbers);
	printf("Total Value: %llu\n", heuristic(numbers));
	
	cout << "Solveable: " << (solveable(numbers)? "true":"false") << endl;
	
	if (solveable(numbers)) {
		solve(numbers);
	}
	
//	uint8_t inversion_array[LINEAR_LENGTH] = {12,1,10,2,7,11,4,14,5,0,9,15,8,13,6,3};
//	cout << "Inversions: " << inversions(inversion_array) << endl;
	
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
	uint8_t depth;
	
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

int max_depth = 0;

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
//		cost += 1;
		cost = (node->cost - cost) + 1;
	
	
	if (child->cost == node->cost) {
		puts("Ending");
	}
#ifdef A_STAR
	child->cost = node->cost + cost;
#else
	child->cost = cost;
//	child->cost = cost + child->depth;
#endif
	
	child->depth = node->depth + 1;
	
//	child->cost = node->cost + heuristic(child->board);
//	child->cost = heuristic(child->board);
	
	return child;
}
#ifdef A_STAR

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

#elif defined BEAM_SEARCH

#define directionAction(direction)\
	Node* child = makeMove(curr, direction, row, col);\
	uint64_t id = uniqueID(child->board);\
	if(ids.count(id)) {\
		delete child;\
	}\
	else {\
		ids.insert(id);\
		priority_nodes.push(child);\
		if (child->cost == 0) {\
			curr = child;\
			break;\
		}\
	}

#else

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
		if (child->cost == 0) {\
			curr = child;\
			break;\
		}\
	}
#endif

//if (child->cost == curr->cost) {\
//if (child->cost == 0) {\

#ifdef BEAM_SEARCH
void solve(uint8_t numbers[LINEAR_LENGTH]) {
	queue<Node*> nodes;
	set<uint64_t> ids;
	
	Node* curr = new Node();
	curr->cost = heuristic(numbers);
	curr->depth = 0;
	memcpy(curr->board, numbers, sizeof(uint8_t) * LINEAR_LENGTH);
	nodes.push(curr);
	
	//while more nodes to explore
	while (nodes.size()) {
//		printf("Node Size: %lu\n", nodes.size());
//		printf("Explored: %lu\n", ids.size());
		priority_queue<Node*, vector<Node*>, comparePointer > priority_nodes;
		
		while (nodes.size()) {
			curr = nodes.front();
			nodes.pop();
			
			uint8_t* curr_numbers = curr->board;
			
			//locate empty space (can be cached)
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
			
			//for each node, expand in each direction
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
		}
		
		if (curr->cost == 0) {
			while (priority_nodes.size()) {
				Node* temp = priority_nodes.top();
				priority_nodes.pop();
				if (temp != curr) {
					delete temp;
				}
			}
			
			break;
		}
		
//		printf("Level Size: %lu\n", priority_nodes.size());
		
		//take top 'BEAM_SIZE' nodes and stick them back in queue
		for (int count = 0; count < BEAM_SIZE && priority_nodes.size(); count++) {
			nodes.push(priority_nodes.top());
			priority_nodes.pop();
		}
		
//		delete the rest
		while (priority_nodes.size()) {
			Node* temp = priority_nodes.top();
			priority_nodes.pop();
			delete temp;
		}
		
	}
	
	//curr is at finishing point
	stack<Node*> solution;
	while (curr != NULL) {
		solution.push(curr);
		curr = curr->parent;
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
	printf("%d moves for solution\n", curr->depth);
	printf("Cost of %d\n", (int) curr->cost);
	printf("%lu nodes explored\n", ids.size());
	
}
#else
void solve(uint8_t numbers[LINEAR_LENGTH]) {
	vector<Node*> nodes;
//	priority_queue<Node*, vector<Node*>, std::greater<Node*> > node_queue; //priority, minimal cost
	priority_queue<Node*, vector<Node*>, comparePointer > node_queue;
	set<uint64_t> ids;
	
	Node* curr = new Node();
	curr->cost = heuristic(numbers);
	curr->depth = 0;
	memcpy(curr->board, numbers, sizeof(uint8_t) * LINEAR_LENGTH);
	node_queue.push(curr);
	
	while (node_queue.size()) {
		curr = node_queue.top();
		node_queue.pop();
		
		uint8_t* curr_numbers = curr->board;
		
		//locate empty space (can be cached)
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
	
	if (node_queue.size() != 0) {
		puts("");
		
		//curr is at finishing point
		stack<Node*> solution;
		while (curr != NULL) {
			solution.push(curr);
			curr = curr->parent;
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
		printf("%d moves for solution\n", curr->depth);
		printf("Cost of %d\n", (int) curr->cost);
	}
	else
		puts("No solution");
	
	
	
	printf("%d nodes explored\n", (int) nodes.size());
	
	
//	11420 nodes explored
//	26 moves for solution
//	Cost of 179
	
//	New Cost function (using A*)
//	10405 nodes explored
//	25 moves for solution
//	Cost of 1021

// New way of doing cost:
//	55 moves for solution
//	Cost of 0
//	219 nodes explored
	
//	4x4 (based off of just the best cost) (best-first search)
//	70 moves for solution
//	Cost of 0
//	5434 nodes explored
	
	//garbage collect
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
}

#endif

#define ODD(num) (num & 1) == 1
#define EVEN(num) (num & 1) == 0

//( (grid width odd) && (#inversions even) )  ||  ( (grid width even) && ((blank on odd row from bottom) == (#inversions even)) )
bool solveable(uint8_t numbers[LINEAR_LENGTH]) {
//	If the grid width is odd, then the number of inversions in a solvable situation is even.
	uint num_inversions = inversions(numbers);
	if (ODD(COLS)) {
		return EVEN(num_inversions);
	}
	else {
//		If the grid width is even, and the blank is on an even row counting from the bottom (second-last, fourth-last etc), then the number of inversions in a solvable situation is odd.
		
//		If the grid width is even, and the blank is on an odd row counting from the bottom (last, third-last, fifth-last etc) then the number of inversions in a solvable situation is even.
		
		int zero_index = 0;
		while (numbers[zero_index] != 0) {
			zero_index++;
		}
		int row_from_bottom = ROWS - (zero_index / COLS);
		return ODD(row_from_bottom) == EVEN(num_inversions);
	}
}

uint inversions(uint8_t numbers[LINEAR_LENGTH]) {
//	uint8_t inversion_array[LINEAR_LENGTH];
//	memset(inversion_array, 0, LINEAR_LENGTH * sizeof(uint8_t));
	
	uint sum = 0;
//	for (int i = LINEAR_LENGTH - 2; i >= 0; i--) {
//		int temp_i = i + 1;
//		while (temp_i < LINEAR_LENGTH && numbers[i] < numbers[temp_i]) {
//			temp_i++;
//		}
//		if (temp_i != LINEAR_LENGTH) {
//			uint8_t temp = inversion_array[temp_i] + 1;
//			inversion_array[i] = temp;
//			sum += temp;
//		}
//	}
//	
	
	for (int i = 0; i < LINEAR_LENGTH; i++) {
		int temp_sum = 0;
		if (numbers[i] == 0) {
			continue;
		}
		for (int inner = i + 1; inner < LINEAR_LENGTH; inner++) {
			if (numbers[i] > numbers[inner] && numbers[inner] != 0) {
				temp_sum++;
			}
		}
//		inversion_array[i] = temp_sum;
		sum += temp_sum;
	}
	
//	for (int i = 0; i < LINEAR_LENGTH; i++) {
//		printf(" %d", inversion_array[i]);
//	}
//	cout << endl;
	
	return sum;
}
