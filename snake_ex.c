#include	<curses.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include    <sys/time.h>
#include	<time.h>
#include    <unistd.h>

#define	SNAKE_HEAD          "0"
#define SNAKE_LEFT_RIGHT    "-"
#define SNAKE_UP_DOWN       "|"
#define BLANK               " "
#define SNAKE_X_SPAWN       0
#define SNAKE_Y_SPAWN       0

void print_to_middle(char *message);
void kill_snake();


int main()
{
	int key;		// user input

	crmode();
	noecho();		//disable printing input
	curs_set(0);    //make the cursor invisible 
	clear();		//empty the screen

	int row     = SNAKE_Y_SPAWN;
	int col     = SNAKE_X_SPAWN;
	int dirY    = 0;
	int dirX    = 0;
	int snake_length;
	int speed = 10;

	initscr();
	keypad(stdscr, TRUE);

	move(row,col);		// get into position
	while(1) //loop for playing the game
	{
		key = getch();
		switch (key)
		{
    		
    		case 'Q':
    		{
    		    break;
    		}
    		
    		case KEY_UP:
    		{
    		    dirY = -1;
    		    dirX = 0;
    		    break;
    		}
    		
    		case KEY_LEFT: 
    		{
    		    dirY = 0;
    		    dirX = -1;
    		    break;
    		}
    		
    		case KEY_RIGHT:
    		{
    		    dirY = 0;
    		    dirX = 1;
    		    break;
    		}
    		
		    case KEY_DOWN: 
    		{
    		    dirY = 1;
    		    dirX = 0;
    		    break;
    		}
		    
		}
		
		if( ( (col + dirX) <= (COLS-2) ) && (row + dirY) <= (LINES-1) && ( (col + dirX) >= 0 ) && (row + dirY) >= 0) //if you're still within the game's borders after your next move
		{
		    move(row,col);
		    if(dirX)             //replace the previous location with the tail
    		    addstr( SNAKE_LEFT_RIGHT );   //add the head at the desired location
    		
    		if(dirY)            //replace the previous location with the tail
    		    addstr( SNAKE_UP_DOWN );
		    
    		row = row + dirY;       //get the desired location's y coordinate 
    		col = col + dirX;       //get the desired location's x coordinate
    		move(row, col);         //move to the desired location
    		
    		addstr( SNAKE_HEAD );
    		
    		move(LINES, COLS);      //park the cursor
    		refresh();              //update the screen
	        usleep(/*1000000*/speed);  //wait for the specified amount of time
    	}
    	
    	else //if you hit a border
    	{
    	    kill_snake();           //call the death method
    	    echo();                 //reactivate the echo
            move(COLS-1, LINES-1);  //park the cursor
    	    return 0;               //end the program
    	} 
	}
	
	endwin();                       //return to the regular terminal
	return 0;                       //end the program
} 

void kill_snake()                   //method for when the snake dies
{
    clear();                        //clear the screen
    char *death = "You died. But it's okay, try again!";    //create a message to be printed
    print_to_middle(death); //run the method to print it to the middle of the screen
    move(COLS-1, LINES-1);  //park the cursor
    endwin();               //end the window
    exit(0);                //exit
    
}

void print_to_middle(char *message) //method for printing methods to the middle of the screen
{
    move( ((LINES/2) - (strlen(message)/2 ) ) , ((COLS/2) - (strlen(message)/2 ) ) );   //find the middle based on the length of the message to print and the size of the screen
    addstr(message);        //put the message there
    move(COLS-1, LINES-1);  //park the cursor
    refresh();              //update the screen
    sleep(4);               //keep the message there for 5 seconds
}

