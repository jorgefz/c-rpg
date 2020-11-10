#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>




/*

	Terminal ASCII GUI

	Version 0.1 - Basic screen matrix is set up
		- Initialising screen: scr_init,
		- Moving cursor: scr_orig, scr_move, scr_displace
		- Display on screen: scr_writeln, scr_clear


	TO-DO

*/


/*
#define scr_print(_ch_max, _format, ...) do{		\
	char _dest[_ch+1];								\
	sprintf(_dest, _ch_max, _format, __VA_ARGS__);	\
	scr_writeln(_dest);								\
} while(0)											\
*/

/* ANSI Escape sequences */
#define UP "\033[A"	//Up one cell
#define DW "\033[B"	//Down one cell
#define FW "\033[C"	//Forward one cell
#define BW "\033[D"	//Back one cell

#define SP " "	//Space
#define NL "\n" //Newline

#define DB "\033[E" //Beginning of line below
#define UB "\033[F" //Beginning of line above




/* Height and width of screen in chars */
static int WIDTH = 1;
static int HEIGHT = 1;

/* Current position of control pointer */
static int X;
static int Y;







/*
Moves the curson to the origin
*/
int scr_orig()
{
	while(X != 0){
		printf(BW);
		X--;
	}
	while(Y != 0){
		printf(UP);
		Y--;
	}
	return 0;
}



/*
Initialise screen system
*/
int scr_init(int w, int h)
{
	if(w <= 0 || h <= 0){
		return 1;
	}

	X = 0;
	Y = 0;

	WIDTH = w;
	HEIGHT = h;
	// Draw square around screen
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<w; j++)
		{
			// Upper & lower limits
			if(i==0 || i==h-1){
				printf("-");
			}
			// Sides
			else if(j == 0 || j == w-1){
				printf("|");
			}
			else{
				printf(" ");
			}
		}
		printf("\n");
	}

	// Align back into screen
	printf(UP);

	X = 0;
	Y = HEIGHT-1;

	scr_orig();

	return 0;
}


/*
Clears all the characters on the screen
*/
int scr_clear()
{
	scr_orig();
	int r = scr_init(WIDTH, HEIGHT);
	return r;
}



/*
Move the stdout control pointer
to a position on the screen
*/
int scr_move(int x, int y)
{
	if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0){
		return 1;
	}

	scr_orig();

	while( X != x){
		printf(FW);
		X++;
	}

	while( Y != y){
		printf(DW);
		Y++;
	}

	return 0;
}



/*
Displaces the cursor by an amount dx and dy
from its current position.
*/
int scr_displace(int dx, int dy)
{
	if( X + dx >= WIDTH || X + dx < 0){
		return 1;
	}

	if( Y + dy >= HEIGHT || Y + dy < 0){
		return 1;
	}

	int nx = X + dx;
	int ny = Y + dy;

	scr_move(nx, ny);

	return 0;
}



/*
Writes a string to the screen
*/
int scr_writeln(const char *line)
{
	if( X + (int)strlen(line) > WIDTH){
		return 1;
	}
	int ch = printf(line);
	X += ch;
	return 0;
}



/*
Writes formatted strings to the screen.
*/
//scr_print(const char *line, ...)





int main(void)
{

	printf("\n\n\n");

	scr_init(50, 25);

	
	scr_writeln("Hello World!");
	
	scr_move(WIDTH/2 - 5, HEIGHT/2);
	scr_writeln("The Center");

	scr_move(WIDTH-1, HEIGHT-1);
	scr_writeln("0");

	scr_clear();

	scr_displace(0, HEIGHT/4);
	scr_writeln("1");

	scr_move(WIDTH-1, HEIGHT-1);

	return 0;
}