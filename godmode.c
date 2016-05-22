/* Library of functions necessary for the A* search algorithm
- priority queue which requires a heap
- nodes, etc*
- createa a priority queue as an array because there a fixed number of nodes*/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <strings.h>
#include <limits.h>
#include "godmode.h"

// CONSTANTS
#define MAX (81) // start with this
#define DIM (9)


// global variables
state* closedSet = NULL; // states already evaluated for taxicab distance - linked list
heap* priorityQ; // priority queue = openSet of discovered but not evaluated states
state* neighbors;
int d = 0; // dimension of the puzzle 
int solved[DIM][DIM]; // solved configuration of puzzle
int possMoves[4]; // possible next states 

// priorityQueue = open set for the A* search algorithm of nodes that have been discovered but not yet evaluated
// A* search algorithm
void Asolve (state* initConfig, int dimension)
{
	// logs the dimensions and solved config of the board for this game
	d = dimension;
	initSolved();


	// initializes openSet of states discovered but not evaluated as a priority queue
	priorityQ = malloc (sizeof(heap));
	priorityQ->nodes = malloc (sizeof(state));
	if (priorityQ == NULL || priorityQ->nodes == NULL)
	{
		printf("sorry could not create priority queue for God Mode\n");
		return;
	}

	priorityQ->len = 1;
	priorityQ->size = 0;

	// calculates cost of initConfig and inserts it into the priorityQ
	initConfig->priority = manhattanDist(initConfig);
	insertState(initConfig, priorityQ);

	state* current;
	//while (priorityQ->len != 1)
	while (1)
	{
		// removes the state with the lowest priority
		current = removeState(priorityQ);
		// check if done
		if(solvedGame(current))
		{
			//TO DO: findSoln(current);
			printf("it's solved!");
			printBoard(current);
			return;
		}
		printf("removed one state from pQ\n");
		printBoard(current);

		// adds to closedSet - linked list
		addClosed(current);
		printf("added to closed Set\n");
		printf("-------\n");

		// finds all the possible next states and added to the priority queue 
		addNeighbors(current);
		printf("finished adding neighbors\n");
		printf("length of priority queue: %d\n", priorityQ->len);


	}

	printf("Ok finished running search algorithm for 3 times\n");
	int i = priorityQ->len + 1;

	 while (i > 1) {
	 	printBoard(&priorityQ->nodes[i]);
	 	i--;

    }
	if (priorityQ->len == 1)
	{
		printf("empty queue!\n");
		// TO DO: freeEverything(priorityQ);
		return;

	}

	/*// need to make it len + 1 because one node is reserved as cursor
	for (int i = 2; i < priorityQ->len + 1; i++)
	{
		printf("priority: %d\n\n", priorityQ->nodes[i].priority);
	} */ 


}

void printBoard(state* config)
{
	printf("Priority: %d\n", config->priority);
	printf("# of prev moves: %d\n", config->moves);
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			printf(" %d ", config->board[i][j]);
		}
		printf("\n");
	}
	printf("\n");

}


// given a configuration calculates the number of moves to the final configuration
// calculates the actual priority
int manhattanDist(state* nextState)
{
	// past moves g(n)
	//int count = 0;
	int count = nextState->moves;
	// iterate through the solved board
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{

			//iterate through config 2d array
			for (int row = 0; row < d; row++)
			{
				for (int col = 0; col < d; col++)
				{
					// keeps track of taxicab distance to correct pos for each tile - f(n)
					if(solved[i][j] == nextState->board[row][col] && solved[i][j] != 0)
					{
						//printf("Solved: %d at %d, %d\nBoard: %d at %d, %d\n", solved[i][j], i , j, nextState->board[row][col], row, col);
						count += abs((col - j) + (row - i));
						//printf("count: %d\n\n", count);

					}

				}
			}



		}
	}
	return count; // g(n) + f(n)
}

// initializes a solved configuration of the board, but not a state
void initSolved(void)
{
	int value = 1;
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			solved[i][j] = value;


			value++;
		}
	}
	solved[d - 1][d - 1] = 0;
}




// determines which moves can be made given where the space is
void addNeighbors(state* config)
{
	/*int brow = 0;
	int bcol = 0;
	// figure out what possible moves there are

		// find where the blank space is
		for (int i = 0; i < d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				if (config->board[i][j] == 0)
				{
					brow = i;
					bcol = j;
				}
			}
		} */ 

		int count = 1;
		state* next;

		while (count < 5)
		{
			// moves Left, Right, Up, and down if possible  and adds them to the priority queue
			next = moveGodMode(config, count);
			if (next != NULL)
			{
				// check that the given board configuration has not already been added to closedSet
				if (!inClosedSet(next))
				{
					printf("not in closedSet\n");
					// iterate over openSet	
					// instead of inOpenSet actually iterate over the priority queue until you find the correct one
					// move from the bottom of the heap
					int i = priorityQ->len + 1; // bottom most child
					int j = i / 2; 
					int found = 0;
					while(i > 1)
					{
						// found the same state in the open set AND the openSet has a lower priority than this one - replace it!!
						if(equalStates(&priorityQ->nodes[i],next) && priorityQ->nodes[i].priority > next->priority)
						{
							printf("reconfiguring existing thing in openSet\n");
							priorityQ->nodes[i].priority = next->priority;
							priorityQ->nodes[i].moves = next->moves;
							priorityQ->nodes[i].prev = next->prev;
							found = 1;
							break;
						}
						i = j;
						j = j / 2;
					}
					/// not found in openSet
					if (found == 0)
					{
						printf("inserting nextConfig\n");
						insertState(next, priorityQ);
					}
				}
			}
			count++;

		}

}





// returns the nextState after moving the blank in a given direction 
// has priority and moves updated
state* moveGodMode(state* config, int dir)
{	


	// allocate memory for new state
	state* nextState = malloc(sizeof(state));
	if (nextState == NULL)
	{
		printf("could not provide next move config\n");
		return NULL;
	}

	int blankRow, blankCol;
	// initialize board
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			nextState->board[i][j] = config->board[i][j];
			if (config->board[i][j] == 0)
			{
				blankRow = i;
				blankCol = j; 
			}
		}
	} 
	//printf("Config before changing the 0: \n");
	
	if (dir == 1 && blankCol != 0)
	{
		printf("move L\n");
		nextState->board[blankRow][blankCol] = nextState->board[blankRow][blankCol - 1];
		nextState->board[blankRow][blankCol - 1] = 0;
	}
	else if (dir == 2 && blankRow != 0)
	{
		printf("move UP\n");
		nextState->board[blankRow][blankCol] = nextState->board[blankRow - 1][blankCol];
		nextState->board[blankRow - 1][blankCol] = 0;
	}
	else if (dir == 3 && blankCol != d - 1)
	{
		printf("move R\n");
		nextState->board[blankRow][blankCol] = nextState->board[blankRow][blankCol + 1];
		nextState->board[blankRow][blankCol + 1] = 0;
	}
	else if (dir == 4 && blankRow != d - 1)
	{
		printf("move DOWN\n");
		nextState->board[blankRow][blankCol] = nextState->board[blankRow + 1][blankCol];
		nextState->board[blankRow + 1][blankCol] = 0;
	}
	else
	{
		free(nextState);
		return NULL;
	}

	// fill in state with values
	nextState->moves = config->moves + 1;
	nextState->priority = manhattanDist(nextState);
	printf("Next State priority: %d\n", nextState->priority);
	printBoard(nextState);
	printf("-----------------\n\n");
	nextState->prev = config;

	return nextState;
}

/* directions
	0 - find where to go;
	1 - left
	2 - up
	3 - right
	4 - down*/ 
/* 
// 1. creates next state given the direction and adds it to the  priority queue open set
void addNextConfig(state* config, int dir, int blankRow, int blankCol)
{
	// allocate memory for new state
	state* nextState = malloc(sizeof(state));
	if (nextState == NULL)
	{
		printf("could not provide next move config\n");
		return;
	}

	// initialize board
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			nextState->board[i][j] = config->board[i][j];
		}
	} 
	//printf("Config before changing the 0: \n");
	
	if (dir == 1)
	{
		printf("move L\n");
		nextState->board[blankRow][blankCol] = nextState->board[blankRow][blankCol - 1];
		nextState->board[blankRow][blankCol - 1] = 0;
	}
	else if (dir == 2)
	{
		printf("move UP\n");
		nextState->board[blankRow][blankCol] = nextState->board[blankRow - 1][blankCol];
		nextState->board[blankRow - 1][blankCol] = 0;
	}
	else if (dir == 3)
	{
		printf("move R\n");
		nextState->board[blankRow][blankCol] = nextState->board[blankRow][blankCol + 1];
		nextState->board[blankRow][blankCol + 1] = 0;
	}
	else if (dir == 4)
	{
		printf("move DOWN\n");
		nextState->board[blankRow][blankCol] = nextState->board[blankRow + 1][blankCol];
		nextState->board[blankRow + 1][blankCol] = 0;
	}

	// fill in state with values
	nextState->moves = config->moves + 1;
	nextState->priority = manhattanDist(nextState);
	printf("Next State priority: %d\n", nextState->priority);
	printBoard(nextState);
	printf("-----------------\n\n");
	nextState->prev = config;
//	printBoard(nextState);

	///// check if already evaluated
	if(inClosedSet(nextState))
	{
		return;
	}
	else
	{  
		// add to priority queue 
		insertState(nextState, priorityQ);
		//if (nextState->prev != NULL)
	//	{
		printf("previous configuraiton of the board: \n");
			printBoard(config->prev);
			printf("----------------------\n\n\n");
		} 

	//} 



} */ 

// returns true if node is already in the closed set
int inClosedSet(state* nextState)
{
	state* cursor = closedSet;
	// make it cursor->prev
	while(cursor->prev != NULL)
	{
		if (equalStates(nextState, cursor))
			return 1;
		//printf("priority: %d\n", cursor->priority);
		cursor = cursor->prev;
	}
	return 0;
}

// returns 0 if they aren't equal states - doesn't matter the previous state, 
// TO DO: only insert if the new state's prioirty is less than all of the other ones
int equalStates(state* st1, state* st2)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			if (st1->board[i][j] != st2->board[i][j])
			{
				return 0;
			}
		}
	}
	return 1;

}



// adds state node to the closed set - just inserts at the beginning
void addClosed(state* config)
{

	// if at beginning of linked list
	if (closedSet == NULL)
	{
		closedSet = config;
		closedSet->prev = malloc(sizeof(state));
		closedSet->prev = config->prev;
	}
	else {
	// has config point to the same as before
	config->prev = closedSet->prev;
	closedSet->prev = config->prev;
	}
	

}

int solvedGame(state* config)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			if(solved[i][j] != config->board[i][j])
			{
				return 0;
			}
		}
	}
	return 1;
}

// inserts a node into the configuration - memry has already been allocated
void insertState(state* config, heap* h)
{

	// no spaces left - need to allocate more space
	if (h->len + 1 >= h->size)
	{
		// size * 2 = space for next level in bin heap, 4 = initial space alloc
		h->size = h->size ? h->size * 2 : 4;
		//array of all of the states currently discovered - adjust memory size allocated
		h->nodes = realloc(h->nodes, h->size * sizeof(state));
	}

	
	int i = h->len + 1; // # of nodes in the binary heap + 1 (reserved as cursor for removingState)
	int j = i / 2; // # of nodes in the lowest level in the binary heap 
	printf("config priority: %d\n", config->priority);

	/* moves from the bottom of the heap while the parent nodes have a lower priority (larger number)
	than the node to be inserted*/ 
	while (i > 1 && h->nodes[i].priority > config->priority)
	{
		// move up to next level in the binary heap
		h->nodes[i] = h->nodes[j];
		i = j;
		j = j / 2;
	}

	// found place to be inserted - place the node there - how does it know how to shift it down?
	h->nodes[i].priority = config->priority;
	//h->nodes[i].dimension = config->dimension;
	// copy board
	for (int k = 0; k < d; k++)
	{
		for (int l = 0; l < d; l++)
		{
			h->nodes[i].board[k][l] = config->board[k][l];
			printf(" %d ",h->nodes[i].board[k][l]);
		}
		printf("\n");
	}
	h->nodes[i].moves = config->moves;
	h->nodes[i].prev = config->prev;
	// frees the state
	//free(config);


	h->len++;
	printf("has been inserted\n");


}

// removes the state with the lowest priority
state* removeState(heap* h)
{

	int i, parent, child; 
	// if nothing to remove return NULL
	if(h->len <= 1)
	{
		return NULL;
	}

	//int d = h->nodes[1].dimension;
	// h->nodes[1] is the bottom most element in the heap 
	// functions as a cursor because it was never filled with a value
	h->nodes[1] = h->nodes[h->len]; // keeps track of the latest function
	h->len--;
	i = 1; // keeps track of which e

	// basically the float down function
	// goes down to the bottom of the heap
	while (1)
	{
		parent = i;
		child = 2 * i;
		// first child - if child is a higher priority, swap the nodes
		if (child <= h->len && h->nodes[child].priority < h->nodes[parent].priority)
		{
			parent = child;
		}
		// second child - if the child is a higher priority, swap the nodes
		if (child + 1 <= h->len && h->nodes[child + 1].priority < h->nodes[parent].priority)
		{
			parent = child + 1;
		}
		// done swapping
		if (parent == i)
		{
			break;
		} 
		// adjusts the curson
		h->nodes[i] = h->nodes[parent];
		i = parent;
	}

	h->nodes[i] = h->nodes[h->len + 1];
	// returns the entire state
	return &h->nodes[1];

}


