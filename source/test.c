

#include <stdio.h>

#include "utils.h"
#include "init.h"




int main(void)
{

	GameData *gd = initGame();
	readGameData(gd);


	//NEXT TASK: choose
	/*
		- Item indexing
		- Read characters from txt
	*/



	freeGame(gd);

	return 0;
}