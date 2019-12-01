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
int snakeLength = 3;
void kill_snake();
void win_snake(); 
void print_to_middle(char *message);
void make_trophy();
void buildWall();
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
    trophyCount = 0;
	row     = 50;		/* start here		*/
	col     = 50;
	
	/* add 1 to row number	*/
	delay = 200;		/* 200ms = 0.2 seconds  */
    buildWall();
    make_trophy();
	move(row,col);		/* get into position	*/
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
	
	    //make_trophy();
	
		if(snakeLength >=  ((COLS * 2) + (LINES * 2)))
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
	
	if(trophyCount > ((rand() % 50) + 10 ))
	{
	    make_trophy();
	}
	
	move( row, col );
	addstr( BLANK );
	//trophyCount++;
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
	else if ( dirY == 1 && row+strlen(SNAKE_HEAD) >= LINES )
		kill_snake();
		
    	
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
    move(COLS-1, LINES-1);  //park the cursor
    endwin();               //end the window
    exit(0);                //exit
    
}

void win_snake()                   //method for when the snake dies
{
    clear();                        //clear the screen
    char *win = "You win! Please play again.";    //create a message to be printed
    print_to_middle(win); //run the method to print it to the middle of the screen
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

void make_trophy()
{
    
    move(trophy_coordinates[0], trophy_coordinates[1]);
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
    
    move(x_coordinate, y_coordinate);	//move to the random pair of coordinates
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







