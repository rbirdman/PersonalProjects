//
//  main.cpp
//  Maze Generator
//
//  Created by Ryan Bird on 6/15/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

#include <iostream>
#include <stack>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <queue>

#define WIDTH 20
#define HEIGHT 20

//#define WIDTH 200
//#define HEIGHT 200


enum DIRECTION {
	NOT_VISITED = 0,
	UP, DOWN, LEFT, RIGHT
	};

bool inBounds(int row, int col) {
	return 0 <= row && row < HEIGHT && 0 <= col && col < WIDTH;
}

//   ____________
//  | | |__ |
//  |
//  |
//  |
//  |
//  |

using namespace std;

class GridEntry {
public:
	
	bool wallLeft, wallTop, wallBottom, wallRight, visited, path;
	GridEntry() : wallLeft(true), wallTop(true), wallBottom(true), wallRight(true), visited(false), path(false) {
	}
	
	vector<DIRECTION> getNeighbors() {
		vector<DIRECTION> neighbors;
		
		if (!wallLeft) {
			neighbors.push_back(LEFT);
		}
		if (!wallRight) {
			neighbors.push_back(RIGHT);
		}
		if (!wallTop) {
			neighbors.push_back(UP);
		}
		if (!wallBottom) {
			neighbors.push_back(DOWN);
		}
		
		return neighbors;
	}
};

typedef struct {
	int row, col;
} Coordinate;
bool inBounds(Coordinate coord) {
	return inBounds(coord.row, coord.col);
}

Coordinate operator+(const Coordinate& first, const Coordinate& second) {
	return {first.row + second.row, first.col + second.col};
}

bool operator!=(const Coordinate& first, const Coordinate& second) {
	return first.row != second.row || first.col != second.col;
}

class Maze {
	GridEntry maze[HEIGHT][WIDTH];
	
	Coordinate start, finish;
	
	//one variable gets random value
	//other variable gets one of edges (0 or HEIGHT/WIDTH - 1)
	Coordinate randomEdgeLocation() {
		int row, col;
		
		if(rand() % 2 == 0) {
			//top or bottom row
			//col is random
			row = (rand() % 2) * (HEIGHT - 1);
			col = rand() % WIDTH;
		}
		else {
			//left or right col
			//row is random
			col = (rand() % 2) * (WIDTH - 1);
			row = rand() % HEIGHT;
		}
		
		return {row, col};
	}
	
	void setWalls(const Coordinate& coord) {
		if (coord.row == 0)
			maze[0][coord.col].wallTop = false;
		else if (coord.row == HEIGHT - 1)
			maze[HEIGHT - 1][coord.col].wallBottom = false;
		
		if (coord.col == 0)
			maze[coord.row][0].wallLeft = false;
		else if (coord.col == WIDTH - 1)
			maze[coord.row][WIDTH - 1].wallRight = false;
	}
	
	void breakWallsBetween(Coordinate& first, Coordinate& second) {
		GridEntry& firstEntry = maze[first.row][first.col];
		GridEntry& secondEntry = maze[second.row][second.col];
		
		//check first: second on top
		if (first.row == second.row + 1) {
			firstEntry.wallTop = false;
			secondEntry.wallBottom = false;
		}
		//check first: second on bottom
		else if (first.row + 1 == second.row) {
			firstEntry.wallBottom = false;
			secondEntry.wallTop = false;
		}
		//check first: second on left
		else if (first.col == second.col + 1) {
			firstEntry.wallLeft = false;
			secondEntry.wallRight = false;
		}
		//check first: second on right
		else if (first.col + 1 == second.col) {
			firstEntry.wallRight = false;
			secondEntry.wallLeft = false;
		}
		else {
			printf("Error! Bad Parameters: (%d,%d) - (%d,%d)\n", first.row, first.col, second.row, second.col);
			exit(1);
		}
	}
	
	void backSolver() {
		queue<Coordinate> edges;
		int path[HEIGHT][WIDTH];
		vector<Coordinate> directions = {
			{-1, 0}, //Up
			{1, 0}, //Down
			{0, -1}, //Left
			{0, 1} //Right
		};
		vector<DIRECTION> oppositePointers = {
			DOWN, UP, RIGHT, LEFT
		};
		
		for (int row = 0; row < HEIGHT; row++)
			for (int col = 0; col < WIDTH; col++)
				path[row][col] = NOT_VISITED;
		
		edges.push(finish);
		if (finish.row == 0)
			path[finish.row][finish.col] = UP;
		else if (finish.row == HEIGHT - 1)
			path[finish.row][finish.col] = DOWN;
		else if (finish.col == 0)
			path[finish.row][finish.col] = LEFT;
		else
			path[finish.row][finish.col] = RIGHT;
		
		while (edges.size()) {
			Coordinate curr = edges.front();
			GridEntry entry = maze[curr.row][curr.col];
			edges.pop();
			
			vector<DIRECTION> directions = entry.getNeighbors();
			for (int i = 0; i < directions.size(); i++) {
				Coordinate neighbor = curr;
				DIRECTION opposite = NOT_VISITED;
				switch (directions[i]) {
					case UP: neighbor.row -= 1; opposite = DOWN; break;
					case DOWN: neighbor.row += 1; opposite = UP; break;
					case LEFT: neighbor.col -= 1; opposite = RIGHT; break;
					case RIGHT: neighbor.col += 1; opposite = LEFT; break;
					default: break;
				}
				if (inBounds(neighbor) && path[neighbor.row][neighbor.col] == NOT_VISITED) {
					edges.push(neighbor);
					path[neighbor.row][neighbor.col] = opposite;
//					path[neighbor.row][neighbor.col] = oppositePointers[i];
				}
			}
			
//			//for each direction, point to self, add each to queue
//			for (int i = 0; i < directions.size(); i++) {
//				Coordinate neighbor = curr + directions[i];
//				if (inBounds(neighbor) && path[neighbor.row][neighbor.col] == NOT_VISITED) {
//					edges.push(neighbor);
//					path[neighbor.row][neighbor.col] = oppositePointers[i];
//				}
//			}
		}
		
		//traverse path
		Coordinate curr = start;
		while (curr != finish) {
			maze[curr.row][curr.col].path = true;
			switch (path[curr.row][curr.col]) {
				case LEFT: curr.col -= 1; break;
				case RIGHT: curr.col += 1; break;
				case DOWN: curr.row += 1; break;
				case UP: curr.row -= 1; break;
					
				default: break;
			}
		}
	}
	
	void breadthFirstSolver() {
		queue<Coordinate> edges;
		int path[HEIGHT][WIDTH];
		
		for (int row = 0; row < HEIGHT; row++) {
			for (int col = 0; col < WIDTH; col++) {
				path[row][col] = NOT_VISITED;
			}
		}
		
		edges.push(start);
		
		while (edges.size()) {
			Coordinate curr = edges.front();
			edges.pop();
			
			//if curr == finish, point to neighbor that is visited
			//break
			
			//for each direction, point to self, add each to queue
			
			
		}
	}
	
	void bidirectionSolver() {
		queue<Coordinate> edges;
		int path[HEIGHT][WIDTH];
		
		for (int row = 0; row < HEIGHT; row++) {
			for (int col = 0; col < WIDTH; col++) {
				path[row][col] = NOT_VISITED;
			}
		}
		
		edges.push(start);
		edges.push(finish);
		
		while (edges.size()) {
			Coordinate curr = edges.front();
			edges.pop();
			
			//if visited from opposite path
			// break and find the two halves to path
			
			//else
			//expand one step in each direction
			//set neighbor's direction to point to self
		}
	}
	
public:
	
	Maze() {
		start = randomEdgeLocation();
		finish = randomEdgeLocation();
		
		printf("Start: %d %d\n", start.row, start.col);
		printf("Finish: %d %d\n", finish.row, finish.col);
		
		setWalls(start);
		setWalls(finish);
	}
	
	void generateMaze() {
		stack<Coordinate> visited;
		Coordinate curr = finish;
		maze[finish.row][finish.col].visited = true;
		maze[finish.row][finish.col].path = true;
		
		vector<Coordinate> directions = {
			{-1, 0}, //Up
			{1, 0}, //Down
			{0, -1}, //Left
			{0, 1} //Right
		};
		
		do {
			//choose random unvisited neighbor
			random_shuffle(directions.begin(), directions.end());
			int i = 0;
			Coordinate neighbor;
			
			for (i = 0; i < directions.size(); i++) {
				neighbor = curr + directions[i];
				if (inBounds(neighbor) && !maze[neighbor.row][neighbor.col].visited) {
					break;
				}
			}
			
			if (i == directions.size()) {
				//all vistors have been visited
				curr = visited.top();
				visited.pop();
			}
			else {
				breakWallsBetween(curr, neighbor);
				visited.push(curr);
				maze[neighbor.row][neighbor.col].visited = true;
				curr = neighbor;
			}
			
		} while (visited.size());
	}
	
	void solveMaze() {
//		bidirectionSolver();
//		breadthFirstSolver();
		backSolver();
	}
//	void printMaze() {
//		//print top row walls
//		GridEntry* firstRow = maze[0];
//		for (int col = 0; col < WIDTH; col++) {
//			GridEntry& currCoord = firstRow[col];
//			printf(" ");
//			if (currCoord.wallTop)
//				printf("_");
//			else
//				printf(" ");
//		}
//		
//		printf("\n");
//		
//		for (int row = 0; row < HEIGHT; row++) {
//			for (int col = 0; col < WIDTH; col++) {
//				GridEntry& currCoord = maze[row][col];
//				
//				if (currCoord.wallLeft)
//					printf("|");
//				else
//					printf(" ");
//				
//				if (currCoord.path) {
//					printf("*");
//				}
//				else
//					printf(" ");
//			}
//			if (maze[row][WIDTH - 1].wallRight)
//				printf("|\n");
//			else
//				printf("\n");
//			
//			//print botton wall
//			printf("|");
//			for (int col = 0; col < WIDTH; col++) {
//				GridEntry& currCoord = maze[row][col];
//				printf(" ");
//				if (currCoord.wallBottom) {
//					printf("-");
//				}
//				else
//					printf(" ");
//			}
//			printf("|\n");
//			
//		}
//	}
	
	void printMaze() {
		//print top row walls
		GridEntry* firstRow = maze[0];
		for (int col = 0; col < WIDTH; col++) {
			GridEntry& currCoord = firstRow[col];
			printf(" ");
			if (currCoord.wallTop)
				printf("_");
			else
				printf(" ");
		}
		
		printf("\n");
		
		for (int row = 0; row < HEIGHT; row++) {
			for (int col = 0; col < WIDTH; col++) {
				GridEntry& currCoord = maze[row][col];
				
				if (currCoord.wallLeft)
					printf("|");
				else
					printf(" ");
				
				
				if (currCoord.wallBottom) {
					if (currCoord.path) {
//						printf("*");
						printf("±");
					}
					else
					//printf("_"); //print * and _ combined somehow
						printf("_");
				}
				else if (currCoord.path) {
//					printf("*");
					printf("+");
				}
				else
					printf(" ");
			}
			if (maze[row][WIDTH - 1].wallRight)
				printf("|\n");
			else
				printf("\n");
			
		}
	}
};

int main(int argc, const char * argv[]) {
	srand(time(NULL));
	int seed = rand();
	seed = 1123672526;
	printf("Seed: %d\n", seed);
	srand(seed);
	
	Maze maze;
	
	maze.generateMaze();
	maze.printMaze();
	
	maze.solveMaze();
	
	maze.printMaze();
	
    return 0;
}

