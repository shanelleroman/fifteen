#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <limits.h>
#include "godMode.h"

int board [3][3];
int value = 1;
int d = 3;

void draw(void)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == 0)
			{
				printf(" _ ");
			}
			else {
				printf(" %d ", board[i][j]);
			}

		}
		printf("\n");
	}
}

int won(void)
{
	int value = 1;
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			if (board[i][j] != value && (i != d- 1 && j != d - 1))
			{
				return 0;
			}
			value++;

		}
	}

	return 1;

}
// my own implementation of getString - utilizes dynamic allocation to return a string of appropriate length
/* Utilizes a buffer, a capacity of the buffer, the length of the buffer actually filled with chars, 
and each character c read in individually
buffer, capacity, length, c*/ 
char* getString(void)
{
	// growable buffer of chars
	char* buffer = NULL;

	// buffer capacity - possible characters that can be filled
	int capacity = 0; 

	// buffer length - actual number of characters used
	int length = 0;

	// character read
	int c;

	while ((c = getchar()) != '\n' && c != EOF)
	{
		//if length  + 1 > capacity (accounts for null terminator)
		if (length  + 1 > capacity)
		{
			// if at the beginning - malloc initial memory
			if (capacity == 0)
			{
				malloc(sizeof(char));
			}
			// if below memory limit - double memory capacity
			else if (capacity < UINT_MAX / 2)
			{
				capacity *= 2;
			}
			// not enough space in memory to allocate for string - returns null as error
			else 
			{
				free(buffer);
				return NULL;
			}

			// reallocates memory in buffer
			char* temp = realloc(buffer, capacity * sizeof(char));
			// check that realloc worked correctly
			if (temp == NULL)
			{
				free(buffer);
				return NULL;
			}
			buffer = temp;

		}
		// copies character into string
		buffer[length++] = c;
	}

	// return null if user provided no input
	if (c == EOF && length == 0)
	{
		return NULL;
	}

	// minimize memory
	char* minimal = realloc(buffer, length * sizeof(char));
	if (minimal == NULL)
	{
		free(buffer);
		return NULL;
	}
	minimal[length] = '\0';
	free(buffer);
	return minimal;

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




int main(int argc, char** argv)
{
	
	

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j< 3; j++)
		{
			board[i][j] = value;
			value++;
			if (i == 2 && j == 2)
			{
				board[i][j] = 0;
			}


		}
	}
	draw();

	if (detSolvable())
	{
		printf("solvable!");
	}
	else {
		printf("not solvable");
	}

	if (won())
	{
		printf("\nyou won!");
	}
	else 
	{
		printf("\nyou haven't won yet");
	}


	printf("%d", atoi("hello"));
	if (strcasecmp("hello", "HELLO") == 0)
	{
		printf("it works");
	}

	



	return 0;
}