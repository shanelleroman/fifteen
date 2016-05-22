/* Godmode library header file for solving the game of fifteen
Implements A* search algorithm 
Shanelle Roman*/
#ifndef GODMODE_H
#define GODMODE_H

// CONSTANTS
#define MAX (81) // start with this
#define DIM (9)



// declares state struct for the priority queue of openset
typedef struct state
{
	int board[DIM][DIM]; // two dimensional board configuration
	int moves; // number of moves from initial board to this boardd config 
	int priority;
	struct state* prev; // the pointer to the previous state

}state;

// creates the heap for priority queue of openset
// pointer to next node
typedef struct heap
{
	state* nodes;
	int len;
	int size;
}heap;

int manhattanDist(state* nextState);
void initSolved(void);
void Asolve (state* initConfig, int dimension);
void insertState(state* config, heap* h);
state* removeState(heap* h);
int solvedGame(state* config);
void addClosed(state* config);
void addNeighbors(state* config);
//void addNextConfig(state* config, int dir, int blankrow, int blankcol);
state* moveGodMode(state* config, int dir);
int inClosedSet(state* nextState);
int equalStates(state* st1, state* st2);
void printBoard(state* config);
//void enterGodMode(int d, int* board);
//void createPQueue(void);


#endif // GODMODE_H 
