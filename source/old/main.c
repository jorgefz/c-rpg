#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utils.h"
#include "vector.h"
#include "setup.h"
#include "inventory.h"
#include "combat.h"
#include "menus.h"

/*

	ARENA - version 0.1

*/





void chooseMonster(Charac *player, size_t playerNum,
					Charac *monsters, size_t monsterNum)
{
	if(playerNum > 1)
		printf(" Wow, more than one!\n");
	while(1)
	{
		clear();
		combatPrintCharac(&player[0]);
		printf(" Choose monster to fight!\n");
		for(size_t i=0; i<monsterNum; i++)
			printf(" %Iu) %s\n", i+1, monsters[i].name);
		printf(" 0) Quit\n");

		int p = getint(0);
		if(p == 0)
			break;
		//Correct for index - option number change
		p--;
		if(p<0 || p>= (int)monsterNum)
			continue;

		//TESTING, two players and three monsters
		Charac testMonsters[3] = {monsters[0],monsters[0],monsters[0]};
		strcpy(testMonsters[1].name, "Stromt 2");
		strcpy(testMonsters[2].name, "Stromt 3");

		Charac testAllies[2] = {player[0], player[0]};
		strcpy(testAllies[1].name, "Player 2");
		
		runCombat(&testAllies[0], 2, &testMonsters[0], 3);
	}
	return;
}


int main(void)
{
	srand(time(NULL));

	printf("\n\n");
	printf(" ============================= \n");
	printf("       Welcome to ARENA 		\n");
	printf(" ============================= \n");
	printf("\n\n");
	printf(" Press enter to continue...");
	getAChar();

	/*
	int saveExists = 0;
	if (checkFile("save.txt"))
		saveExists = 1;
	*/

	while(1)
	{
		clear();
		printf(" ============================= \n");
		printf("           ARENA \n");
		printf(" ============================= \n");
		printf(" 1) New Game\n");
		printf(" 0) Quit\n");
		char option = getAChar();
		if(option == '0')
			return 0;
		else if(option == '1')
			break;
		else
			continue;
	}

	//Initilaisation
	size_t raceNum;
	Race *races = readRaces("races.txt", &raceNum);
	if(!races)
		return 0;
	size_t itemNum;
	Item *itemList = readItems("items.txt", &itemNum);
	if(!itemList)
		return 0;

	size_t monsterNum;
	Charac *monsters = readCharac("monsters.txt", &monsterNum);
	if(!monsters)
		return 0;

	//Give swords to monsters
	/*
	Item *sword = searchItemByName(itemList, itemNum, "Sword");
	for(size_t i=0; i<monsterNum; i++){
		addToInv(monsters[i].inv, sword);
		equipItem(&monsters[i], 0);
	}
	*/


	//Setup player
	Charac *playerList = menuPlayerSetup(races, raceNum, itemList, itemNum);
	size_t playerNum = 1;
	if(!playerList)
		return 0;

	//Game
	menuInventory(&playerList[0]);
	//chooseMonster(playerList, playerNum, monsters, monsterNum);

	free(playerList);
	//Free inventories
	for(size_t i=0; i<playerNum; i++)
		vfree(playerList[i].inv);
	
	free(races);
	free(monsters);
	free(itemList);

	return 0;
}