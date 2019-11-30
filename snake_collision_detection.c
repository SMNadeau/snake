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
int visited_check(int visited_array[][2], int move, int col, int row);


int main()
{
	int key;		// user input

	crmode();
	noecho();		//disable printing input
	curs_set(0);    //make the cursor invisible 
	clear();		//empty the screen

	int row     = SNAKE_Y_SPAWN;
	int col     = SNAKE_X_SPAWN;
	int desired_row;
	int desired_col;
	int dirY    = 0;
	int dirX    = 0;
	int snake_length;
	int speed = 10;

	initscr();
	keypad(stdscr, TRUE);

    int visited_array[9999][2];//[COLS*LINES][1];

	
	for(int i = 0; i >= 0; i++) //loop for playing the game
	{
	    move(row,col);		        // get into the new position
	    addstr( SNAKE_HEAD );       // draw the starting head
	    visited_array[i][0] = col;  // record this position in the visited array
		visited_array[i][1] = row;  // record this position in the visited array
		key = getch();              // get next key input
		switch (key)                // look at relevant key options
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
		
		desired_row = row + dirY;   //calculate what the next row will be after moving
		desired_col = col + dirX;   //calculate what the next column will be after moving
		
		if( ( (desired_col) <= (COLS-2) ) && (desired_row) <= (LINES-1) && ( (desired_col) >= 0 ) && (desired_row) >= 0) //if you're still within the game's borders after your next move
		{
		    if( (visited_check(visited_array, i, desired_col, desired_row)) ) //if your next move is to an already visited location
		    {
    		    kill_snake();           //call the death method
        	    echo();                 //reactivate the echo
                move(COLS-1, LINES-1);  //park the cursor
        	    return 0;               //end the program

		    }
		    else //if the next move is valid
		    {
    		    move(row,col);  //move to the current location
    		    if(dirX)        // if it's moving horizontally
    		        addstr( SNAKE_LEFT_RIGHT ); //use a horizontal tail
        		
        		if(dirY)        //if it's moving vertically 
        		    addstr( SNAKE_UP_DOWN );    //use the vertical tail
    		    
        		row = desired_row;  //upate the current row value to the new projected row 
        		col = desired_col;  //update the current row value to the new projected column
        		move(row, col);         //move to the desired location
        		
        		addstr( SNAKE_HEAD );   //put the head there
        		
        		move(LINES, COLS);      //park the cursor
        		refresh();              //update the screen
    	        usleep(1000000/speed);  //wait for the specified amount of time
		    }
		    
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

int visited_check(int visited_array[][2], int move, int col, int row) //returns 1 on visited
{
    for(int i = 0; i <= move; i++)  //look at every move that's been made so far
    {
        if( (visited_array[i][0] == col) && (visited_array[i][1] == row) ) //if the col matches one already in the list 
        {
                return 1; //this location is already visited,  return 1 or true
        }
    }
    return 0; //if the entire array is gone through and it's not visited, return 0 or false
}