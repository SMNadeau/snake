#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	<curses.h>
#include    <unistd.h>



void make_trophy(int max_domain, int max_range);
char randString[] = "1359";
int main()
{
    time_t t; //declare time
    srand((unsigned) time(&t)); //initialize randomizer

	initscr(); //start window
	clear(); //make sure it's empty
    
    while(1)
    {
        make_trophy(COLS-1, LINES-1);
        usleep(1000000);
    }
    
    return 0;
}

void make_trophy(int max_domain, int max_range)
{
    int x_coordinate;
    int y_coordinate;
    int value;
    char chosen[1];
    
    x_coordinate = rand() % max_domain; //max_domain should probably be COLS
    y_coordinate = rand() % max_range;  //max_range should probably be LINES
	move(x_coordinate, y_coordinate);	//move to the random pair of coordinates
	value = rand() % 4;
	chosen[0] = randString[value];
	addstr(chosen);                        //'x' represents the new spawned food. Can be replaced later
	move(COLS-1, LINES-1);              //park the cursor
	refresh();                          //show changes made to the screen
}
