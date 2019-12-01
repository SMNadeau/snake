#include	<stdio.h>
#include	<signal.h>
#include    <sys/time.h>
#include	<string.h>
#include	<curses.h>
#include	<stdlib.h>
#include	<time.h>
#include    <unistd.h>

#define	SNAKE_HEAD          "0"
#define SNAKE_LEFT_RIGHT    "-"
#define SNAKE_UP_DOWN       "|"
#define BLANK               " "
#define SNAKE_X_SPAWN       0
#define SNAKE_Y_SPAWN       0

int set_ticker( int n_msecs );

int	row;	/* current row		*/
int	col;	/* current column	*/
int	dirY;	/* where we are going	*/
int	dirX;

void kill_snake();
void print_to_middle(char *message);

//int visited_check(int visited_array[][2], int move, int col, int row);
//int make_trophy(int visited_array[][2], int max_domain, int max_range, int trophy_coordinates[],int i);

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
    
	row     = 50;		/* start here		*/
	col     = 50;
	
	/* add 1 to row number	*/
	delay = 200;		/* 200ms = 0.2 seconds  */

	move(row,col);		/* get into position	*/
	addstr(SNAKE_HEAD);	/* draw message		*/
	signal(SIGALRM, move_msg );
	set_ticker( delay );

	while(1)
	{
	    /*
		if(!trophy_on_screen)
	    {
	        trophy_value = make_trophy(visited_array, COLS-2, LINES-1, trophy_coordinates, i);
	        trophy_on_screen = 1;
	    }
		*/
		
		ndelay = 0;
		c = getch();
		switch (c)                // look at relevant key options
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
		if ( ndelay > 0 )
		{
		    set_ticker( delay = ndelay );
		}
	}
	endwin();
	return 0;
}


void move_msg(int signum)
{
	signal(SIGALRM, move_msg);	// reset, just in case	
	move( row, col );
	addstr( BLANK );
	col += dirX;
	row += dirY;  //move to new column	
	move( row, col );		// then set cursor	
	addstr( SNAKE_HEAD );		// redo message		
	refresh();			// and show it		
	
	 // now handle borders
	
	if ( dirX == -1 && col <= 0 )
		kill_snake();
	else if ( dirX == 1 && col+strlen(SNAKE_HEAD) >= COLS )
		kill_snake();
		
		if ( dirY == -1 && row <= 0 )
		kill_snake();
	else if ( dirY == 1 && row+strlen(SNAKE_HEAD) >= COLS )
		kill_snake();
}


/*
 *      set_ticker.c
 *          set_ticker( number_of_milliseconds )
 *                   arranges for the interval timer to issue
 *                   SIGALRM's at regular intervals
 *          returns -1 on error, 0 for ok
 *
 *      arg in milliseconds, converted into micro seoncds
 */


int set_ticker( int n_msecs )
{
        struct itimerval new_timeset;
        long    n_sec, n_usecs;

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

int make_trophy()
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
}