#include	<stdio.h>
#include	<signal.h>
#include    <sys/time.h>
#include	<string.h>
#include	<curses.h>
#include	<stdlib.h>
#include	<time.h>
#include    <unistd.h>

#define	SNAKE_HEAD          "o"
#define SNAKE_LEFT_RIGHT    "-"
#define SNAKE_UP_DOWN       "|"
#define BLANK               " "
#define SNAKE_X_SPAWN       0
#define SNAKE_Y_SPAWN       0

int set_move_ticker( int n_msecs );
//int set_trophy_ticker( int n_msecs );
int	row;	/* current row		*/
int	col;	/* current column	*/
int	dirY;	/* where we are going	*/
int	dirX;
int trophy_coordinates[2];
int trophyCount;
int iteration = 0;
int snake_length = 3;
int visited_array[9999][2];
int trophyVal = 0;

int visited_check(int move, int col, int row); //returns 1 on visited
void kill_snake();
void win_snake(); 
void print_to_middle(char *message);
void make_trophy();
void buildWall();

// Snake is defined
struct snake
{
   struct snake* next;
   int x;
   int y;
};

int main()
{
	time_t t;
	srand((unsigned) time(&t));

	int	delay;		/* bigger => slower	*/
	int	ndelay;		/* new delay		*/
	int	c;		/* user input		*/
	void	move_msg(int);	/* handler for timer	*/
	
	int d = rand() % 4;
	switch (d)                // look at relevant key options
		{
    		case 0:
    		{
    		    dirY = -1;
    		    dirX = 0;
    		    break;
    		}
    		
    		case 1: 
    		{
    		    dirY = 0;
    		    dirX = -1;
    		    break;
    		}
    		
    		case 2:
    		{
    		    dirY = 0;
    		    dirX = 1;
    		    break;
    		}
    		
		    case 3: 
    		{
    		    dirY = 1;
    		    dirX = 0;
    		    break;
    		}
		}
	initscr();
	crmode();
	noecho();
	clear();
    curs_set(0); 
    keypad(stdscr, TRUE);
    trophyCount = 0;
	
	/* add 1 to row number	*/
	delay = 1000;		/* 200ms = 0.2 seconds  */
    buildWall();
    make_trophy();
	row     = 50;		/* start here		*/
	col     = 50;
	move(row, col);		/* get into position	*/
	addstr(SNAKE_HEAD);	/* draw message		*/
	signal(SIGALRM, move_msg );
	set_move_ticker( delay );
    
	while(1)
	{
		
		ndelay = 0;
		c = getch();
		switch (c)                // look at relevant key options
		{
    		
    		case 'Q':
    		{
    		    kill_snake();
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
		if ( ndelay > 0 )
		{
		    set_move_ticker( delay = ndelay );
		}
	
	
		if(snake_length >=  ((COLS * 2) + (LINES * 2)))
		{
		    win_snake();
		}
	}
	endwin();
	return 0;
}


void move_msg(int signum)
{
    		
	signal(SIGALRM, move_msg);	// reset, just in case	
	trophyCount++;
	int trophy_timer = (rand() % 50) + 50; 
	
	if((trophyCount > trophy_timer)) //if the counter for how long the trophy has been on screen exceeds the time it was supposed to last
	{
	    make_trophy(); //make a new one
	}
	
	
	int desired_row = row + dirY;   //calculate what the next row will be after moving
	int desired_col = col + dirX;   //calculate what the next column will be after moving
	
    if(iteration >= snake_length) //make sure the tail doesn't exceed what should be the length of the snake
    {
            move( visited_array[ (iteration - snake_length - 1)][1], visited_array[(iteration - snake_length - 1)][0]); //move to the spot after the tail
            
            addstr( BLANK ); //replace the tail with a blank space
            visited_array[(iteration - snake_length - 1)][0] = 0; //clear the values from the array
            visited_array[(iteration - snake_length - 1)][1] = 0; //clear the values from the array
     }
	if( ( (desired_col) <= (COLS-2) ) && (desired_row) <= (LINES-1) && ( (desired_col) >= 0 ) && (desired_row) >= 0) //if you're still within the game's borders after your next move
	{
        	if( (visited_check(iteration, desired_col, desired_row)) ) //if your next move is to an already visited location
		    {
    		    kill_snake();           //call the death method
        	    echo();                 //reactivate the echo
                move(LINES-1, COLS-1);  //park the cursor
		    }	
		    
        	else //if the next move is valid
        	{
        	move( row, col );
        	if(dirX)        // if it's moving horizontally
                addstr( SNAKE_LEFT_RIGHT ); //use a horizontal tail
            if(dirY)        //if it's moving vertically 
                addstr( SNAKE_UP_DOWN );    //use the vertical tail
            
        	col += dirX;    //move to the new column
        	row += dirY;    //move to new row
        	move( row, col );		// then set cursor	
        	addstr( SNAKE_HEAD );		// redo message	
		    visited_array[iteration][0] = col;  // record the x coordinate in the visited array
    		visited_array[iteration][1] = row;  // record the y coordinate in the visited array
    		if( (col == trophy_coordinates[0]) && (row == trophy_coordinates[1]) )
    		{
    		    snake_length += trophyVal; //add the trophy's value to the snake's length
	    	    make_trophy(); //make a new trophy

    		}

        	refresh();	// and show it	
    	    iteration++; //increment the counter for the number of moves the snake has made

        	}
    	
	}
    else //if you hit a border
	{
	    kill_snake();           //call the death method
	    echo();                 //reactivate the echo
        move(LINES-1, COLS-1);  //park the cursor
	}
	
}


int set_move_ticker( int n_msecs )
{
        struct itimerval new_timeset;
        long    n_sec, n_usecs, sec;
        
        n_sec = n_msecs / 1000 ;
        n_usecs = ( n_msecs % 1000 ) * 1000L ;


        new_timeset.it_interval.tv_sec  = n_sec;        /* set reload  */
        new_timeset.it_interval.tv_usec = n_usecs;      /* new ticker value */
        new_timeset.it_value.tv_sec     = n_sec  ;      /* store this   */
        new_timeset.it_value.tv_usec    = n_usecs ;     /* and this     */
        

	    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}



void kill_snake()                   //method for when the snake dies
{
    clear();                        //clear the screen
    char *death = "You died. But it's okay, try again!";    //create a message to be printed
    print_to_middle(death); //run the method to print it to the middle of the screen
    move(LINES-1, COLS-1);  //park the cursor
    endwin();               //end the window
    exit(0);                //exit
    
}

void win_snake()                   //method for when the snake dies
{
    clear();                        //clear the screen
    char *win = "You win! Please play again.";    //create a message to be printed
    print_to_middle(win); //run the method to print it to the middle of the screen
    move(LINES-1, COLS-1);  //park the cursor
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


int visited_check(int move, int col, int row) //returns 1 on visited
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


void make_trophy()
{
    
    move(trophy_coordinates[1], trophy_coordinates[0]);
    addstr(" ");
    int x_coordinate = rand() % (COLS - 2); 
    int y_coordinate = rand() % (LINES - 1);  
    int trophy_picker = rand() % 4;         //this will be the randomized value that'll be used to determine the  value of the trophy
    char trophy_value[3]; //value that'll be returned as the value of the trophy
    
    switch (trophy_picker)
    {
        case 0: // if the randomizer returns 0
        {
            sprintf(trophy_value, "%s", "1");
            break;
        }
        case 1: // if the randomizer returns 1
        {
            sprintf(trophy_value, "%s", "3");
            break;
        }
        case 2: // if the randomizer returns 2
        {
            sprintf(trophy_value, "%s", "5");
            break;
        }
        case 3: // if the randomizer returns 3
        {
            sprintf(trophy_value, "%s", "9");
            break;
        }
    }
    trophyVal = atoi(trophy_value);
    move(y_coordinate, x_coordinate);	//move to the random pair of coordinates
	addstr(trophy_value);
	                        //'x' represents the new spawned food. Can be replaced later
	refresh(); 
    
    trophy_coordinates[0] = x_coordinate;
    trophy_coordinates[1] = y_coordinate;
    
    trophyCount = 1;
    return;
}


void buildWall()
{
    for(int i = 0; i <= LINES; i++)
    {
        move(i, 0);
        addstr("|");
        
        move(i, (COLS - 1));
        addstr("|");
    }
    
    for(int i = 0; i <= COLS; i++)
    {
        move(0, i);
        addstr("_");
        
        move((LINES - 1), i);
        addstr("_");
    }
    
    refresh();
}
