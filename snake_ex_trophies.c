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
int make_trophy(int visited_array[][2], int max_domain, int max_range, int trophy_coordinates[],int i);


int main()
{
    time_t t; //declare time
    srand((unsigned) time(&t)); //initialize randomizer for the trophy
    
    int trophy_on_screen = 0;   //flag for checking whether there's a trophy on the screen for the snake to go after
    int trophy_value;   //value that determines how much longer the snake gets after collecting the trophy
    
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
    int trophy_coordinates[2];
	
	for(int i = 0; i >= 0; i++) //loop for playing the game
	{
	    if(!trophy_on_screen)
	    {
	        trophy_value = make_trophy(visited_array, COLS-2, LINES-1, trophy_coordinates, i);
	        trophy_on_screen = 1;
	    }
	    
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
                if( (row == trophy_coordinates[0]) && (col == trophy_coordinates[1]) ) //if the next move is the trophy
		        {
		            trophy_on_screen = 0;
		        }
		        

        		move(LINES, COLS);      //park the cursor
        		refresh();              //update the screen
    	        usleep(/*1000000*/speed);  //wait for the specified amount of time
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


int make_trophy(int visited_array[][2], int max_domain, int max_range, int trophy_coordinates[], int i)
{
    int x_coordinate = rand() % max_domain; //max_domain should probably be COLS
    int y_coordinate = rand() % max_range;  //max_range should probably be LINES
    int trophy_picker = rand() % 4;         //this will be the randomized value that'll be used to determine the  value of the trophy
    int trophy_value; //value that'll be returned as the value of the trophy
    
    switch (trophy_picker)
    {
        case 0: // if the randomizer returns 0
        {
            trophy_value = 1;
            break;
        }
        case 1: // if the randomizer returns 1
        {
            trophy_value = 3;
            break;
        }
        case 2: // if the randomizer returns 2
        {
            trophy_value = 5;
            break;
        }
        case 3: // if the randomizer returns 3
        {
            trophy_value = 9;
            break;
        }
    }
    

	while( (visited_check(visited_array, i, x_coordinate, y_coordinate)) ) //if the new trophy will spawn on the snake
	{
	    x_coordinate = rand() % max_domain; //keep randomizing new coordinates
        y_coordinate = rand() % max_range;  //until they no longer spawn on the snake
	}
	
	move(x_coordinate, y_coordinate);	//move to the random pair of coordinates
	
	addstr("x");                        //'x' represents the new spawned food. Can be replaced later
	move(COLS-1, LINES-1);              //park the cursor
	refresh();                          //show changes made to the screen
    trophy_coordinates[0] = x_coordinate;
    trophy_coordinates[1] = y_coordinate;

    return trophy_value;
}



