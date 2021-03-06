/* Game of fifteen: hacker version which includes god mode*/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <strings.h>
#include <limits.h>
#include "godmode.h"

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];
int solved[DIM_MAX][DIM_MAX];

//blank space position
int x;
int y;

// dimensions
int d;

// prototypes
void clear(void); // clears the screen
void greet(void);
void init(void);
int detSolvable(void); // determines whether initial config is solvable
void draw(void);  // draws the board
int move(int tile); // moves a given tile
int won(void); // determines whether it's won or not
//void readInput(void);
char* getString(void);
int enterGodMode(void);
//void initState(int board [], int d);
state* initState(void);



int main(int argc, char** argv)
{
	// default initialization of 3 x 3 board
	if (argc == 1)
	{
		d = 3;
	}
	// reads in command line argument for setting board dimensions
	else
	{
		d = atoi(argv[1]);
	}

	// checks for correct dimensions
	if (d < DIM_MIN || d > DIM_MAX)
	{
		printf("Dimensions must be between 3x3 and 9x9\n");
		return 1;
	}

	// set up board
	clear();
	greet();
	srand48(1);
	init();

	// open a file for log
	FILE* fp = fopen("log.txt", "w");
	assert(fp);


	while (!won())
	{
		// clears screen and draws new board

		draw();


		// read Input and moves tile
		char* string;
		// gets String until it's null
		while ((string = getString()) != NULL)
		{
			int tile = atoi(string);
			// enter godMode
			if (tile == 0 && (strcasecmp("GOD", string) == 0))
			{
				printf("should enter god mode\n");
				enterGodMode();
				return;
			}
			// invalid input - either string or wrong integer - enters while loop again
			else if (tile < 1 || tile > d * d - 1)
			{
				printf("Invalid move - try again!\n");
				draw();
			}
			else if(!move(tile))
			{
				printf("Invalid move - try again!\n");
				draw();
			}
			// valid input - moves tile
			else
			{
				break;
			}
		}


	}

	printf("Congrats on winning!\n");



}

// Clears the terminal view with ANSI escape sequences
void clear(void)
{
	printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

// clears the screen and greets the user
void greet(void)
{
	clear();
	printf("Welcome to game of fifteen!\n");
	usleep(20000);
}

void draw(void)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			if(board[i][j] == 0)
			{
				printf(" _ ");
			}
			else 
			{
				printf(" %d ", board[i][j]);
			}
		}
		printf("\n");
	}

}

// moves the tile
int move(int tile)
{
	// look for the tile with value tile
	// check if neighboring tile has value 0 (x, y) are global variables
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			//printf("x: %d\ny: %d\ni: %d\nj: %d\n\n", x, y, i, j);
			// found tile
			if (board [i][j] == tile)
			{
				// move tile if valid move
				if ((i - 1 == x && j == y) || (i + 1 == x && j == y) || (i == x && j - 1 == y) || (i == x && j + 1 == y))
				{
					// switch tiles
					board[x][y] = board[i][j];
					board[i][j] = 0;
					// shift position of blank tile
					x = i;
					y = j;
					return 1;
				}

			}

		}
	}


	return 0;

}

/* Initializes the board with pseudorandom but solvable configuration
- Starts with correct board
- Generates random number of moves and tiles to switch
*/ 
void init()
{
	// start with correct configuration
	int value = 1; 
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			board[i][j] = value;
			solved[i][j] = value;
			value++;

		}
	}
	board[d - 1][d - 1] = 0;
	// use solved in Godmode
	solved[d - 1][d - 1] = 0;
	x = d - 1;
	y = d - 1;

	// seed the pseudorandom# generator
	srand(time(0));
	// random number of moves
	int numberMoves = rand() % 100 + 1;
	int count = 0;
	int tile; 
	// generates a solvable configuration by completing an random number of mvoes
	while (count < numberMoves)
	{
		// generates a "random" tile to move until number generated borders a space
		tile = (rand() % (d * d)) + 1;
		// if the game is won, moves the tile again to make sure it's in puzzle config
		while (!move(tile) || won())
		{
			tile = (rand() % (d * d)) + 1;
		}

		// keeps track of number moves completed
		count++;		

	}

	// checks that it is solvable
	if(!detSolvable())
	{	
		printf("Oops! There was an error in board generation. Please wait a moment\n");
		init();
	}


} 



// given an initial board configuration - checks whether or not it's solvable
int detSolvable(void)
{
	int parity = 0;
    int blankRow = 0; // the row with the blank tile

    for (int i = 0; i < d; i++)
    {
    	for (int j = 0; j < d; j++)
    	{
    		// if encounter blank tile
    		if (board[i][j] == 0)
    		{
    			blankRow = i;
    		}


    		// determine number of inversions
    		int testrow = i;
    		int testcol = j;
    		while (testrow != d)
    		{	
    			if (board[i][j] > board [testrow] [testcol] && board [testrow][testcol] != 0)
    			{
    				parity++;
    			}
    			testcol++;
    			if (testcol == d)
    			{
    				testcol = 0;
    				testrow++;
    	
    			}
    			
    		}
    	}
    }

    // odd width and blank on even row from the bottom eg, second last --> solvable
    if ((d % 2 == 1) && (parity % 2 == 0))
    {
    	return 1;
    }
    // even width and blank on __ row  from bottom and  ___ parity (must match - even/odd) --> solvable
    else if ((d % 2 == 0) && ((d - blankRow) % 2 == parity % 2))
    {
    	return 1;
    }
    return 0;

}

// calls the A* search algorithmn, implemented in godmode.c library
// passes in the dimension of the maze to determine # of possible nodes in the priority queue

/* Read input function - 
A. can trigger an entry into godmode 
B. moves the tile */ 
void readInput(void)
{



	
}




// my own implementation of getString - utilizes dynamic allocation to return a string of appropriate length
/* Utilizes a buffer, a capacity of the buffer, the length of the buffer actually filled with chars, 
and each character c read in individually
buffer, capacity, length, c*/ 
char* getString(void)
{
    // growable buffer for chars
    char* buffer = NULL;

    // capacity of buffer
    unsigned int capacity = 0;

    // number of chars actually in buffer
    unsigned int n = 0;

    // character read or EOF
    int c;

    // iteratively get chars from standard input
    while ((c = fgetc(stdin)) != '\n' && c != EOF)
    {
        // grow buffer if necessary
        if (n + 1 > capacity)
        {
            // determine new capacity: start at 32 then double
            if (capacity == 0)
            {
                capacity = 32;
            }
            else if (capacity <= (UINT_MAX / 2))
            {
                capacity *= 2;
            }
            else
            {
                free(buffer);
                return NULL;
            }

            // extend buffer's capacity
            char* temp = realloc(buffer, capacity * sizeof(char));
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        // append current character to buffer
        buffer[n++] = c;
    }

    // return NULL if user provided no input
    if (n == 0 && c == EOF)
    {
        return NULL;
    }

    // minimize buffer
    char* minimal = malloc((n + 1) * sizeof(char));
    strncpy(minimal, buffer, n);
    free(buffer);

    // terminate string
    minimal[n] = '\0';

    // return string
    return minimal;

}

/* enterGodMode = A* search algorithm
couldn't figure out how to pass in a two dimensional array 
so... creating initial state right now in enterGodMode()
returns true if it's solved, returns false if it's not
*/ 
int enterGodMode(void)
{
	// creates initial state of the board to place into openSet priority queue
	state* start = initState();
	if (start == NULL)
	{
		printf("God mode has failed. Please solve on your own!\n");
		return 0;
	}
	Asolve(start, d);

	return 0;

}


// returns 1 when the game is over and you have won! 
int won(void)
{
	int count=1;
    
    //iterate through multi-D array
    for(int i=0; i<d; i++)
    {
        for(int j=0; j<d; j++)
        {
            //check if 1-8 ascending
            if(board[i][j]==count)
            {
                count++;
            }
            //check it's not the blank
            else if(board[i][j]!=0)
            {
                return 0;
            }
        }
    }

    
    if(board[d-1][d-1]==0)
    {
        return 1;
    }
    
    return 0;

}

//void initState(int board [], int d)
state* initState(void)
{
	// create initial state for the priority queue
	state* init_state = malloc(sizeof(state));
	if (init_state == NULL)
	{

		return init_state;

	}
	// fill in initial board configuration
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			init_state->board[i][j] = board [i][j];
		}
		printf("\n");
	}
	// fill in additional info
	//init_state->dimension = d;
	init_state->moves = 0;
	init_state->prev = NULL;
	return init_state;

}



