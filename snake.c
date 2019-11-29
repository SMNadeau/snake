/* bounce1d.c
 *	purpose	animation with user controlled speed and direction
 *	note the handler does the animation
 *	the main program reads keyboard input
 *
 *	user input: 
 *  s slow down 
 *	f speed up
 *  space reverse direction 
 *	Q quit
 *
 *	compile	it as follows: 
 *	gcc bounce1d.c -lcurses -o bounce1d
 */
#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include    <sys/time.h>
#include	<string.h>

/* some global settings main and the handler use */

#define	MESSAGE	"0"
#define BLANK   "     "

int set_ticker( int n_msecs );

int	row;	/* current row		*/
int	col;	/* current column	*/
int	dirY;	/* where we are going	*/
int	dirX;

int main()
{
	int	delay;		/* bigger => slower	*/
	int	ndelay;		/* new delay		*/
	int	c;		/* user input		*/
	void	move_msg(int);	/* handler for timer	*/

	initscr();
	crmode();
	noecho();
	clear();

	row     = 50;		/* start here		*/
	col     = 50;
	dirY    = -1;
	dirX    = 0;
	/* add 1 to row number	*/
	delay = 200;		/* 200ms = 0.2 seconds  */

	move(row,col);		/* get into position	*/
	addstr(MESSAGE);	/* draw message		*/
	signal(SIGALRM, move_msg );
	set_ticker( delay );

	while(1)
	{
		ndelay = 0;
		c = getch();
		if ( c == 'Q' )
		{
		    break;
		}
		if ( c == 'w' )
		{
		    dirY = -1;
		    dirX = 0;
		}
		if ( c == 'a')
		{
		    dirY = 0;
		    dirX = -1;
		}
		if ( c == 's' )
		{
		    dirY = 1;
		    dirX = 0;
		}
		if ( c == 'd' )
		{
		    dirY = 0;
		    dirX = 1;
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
	signal(SIGALRM, move_msg);	/* reset, just in case	*/
	move( row, col );
	addstr( BLANK );
	col += dirX;
	row += dirY; /* move to new column	*/
	move( row, col );		/* then set cursor	*/
	addstr( MESSAGE );		/* redo message		*/
	refresh();			/* and show it		*/

	/*
	 * now handle borders
	 */
	if ( dirX == -1 && col <= 0 )
		dirX = 1;
	else if ( dirX == 1 && col+strlen(MESSAGE) >= COLS )
		dirX = -1;
		
		if ( dirY == -1 && row <= 0 )
		dirY = 1;
	else if ( dirY == 1 && row+strlen(MESSAGE) >= COLS )
		dirY = -1;
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
 
