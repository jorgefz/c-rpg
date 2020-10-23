#ifndef _SETUP_HEADER_H
#define _SETUP_HEADER_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#include "utils.h"

/*

	This file contains:
		- Game Structures
		- Data reading and writing functions
		- Initialisers

	TODO:
		Finish readPlayer function
		Finish saveGame function
		Clean read fucntions

*/



// GAME STRUCTURES

// Generic Items
typedef struct itemStruct{
	size_t ID;
	char name[32];
	//Item descriptors; see inventory.c
	char type;
	char type2;
	
	uint priceGold;
	//Rolltype if weapon, health if hpotion, etc
	int data;
} Item;

// Characters
typedef struct charStruct{
	char name[100];
	char race[100];
	//Active Stats
	int HP;
	int maxHP;
	int SP;
	int maxSP;
	int MP;
	int maxMP;
	//Passive Stats
	int cons;
	int strn;
	int dext;
	int intl;
	//Data
	int lvl;
	int currentExp;
	int nextLvlExp;
	int gold;
	//Equipment Slots
	Item *mainHand;
	Item *boots;
	Item *trousers;
	Item *chest;
	Item *head;
	//Item Inventory
	size_t itemNum;
	size_t maxInvSize;
	Item *inv;
	//Magic
	//size_t spellNum;
	//Magic spells[maxSpells];
} Charac;


// Player Races
typedef struct raceStruct
{
	char name[100];
	//1-yes, 0-no
	int playable;
	//Modifiers
	int cons;
	int strn;
	int dext;
	int intl;
} Race;




//			FUNCTIONS


/*
Returns pointer to first item that matches input name 'itemName'.
If no instances are found, NULL is returned.
*/
Item *searchItemByName(Item *itemList, size_t itemNum,
						const char *itemName);

/*
Returns pointer to first item that matches input ID.
If no instances are found, NULL is returned.
*/
Item *searchItemByID(Item *itemList, size_t itemNum, 
					size_t inputID);

/*
Gathers all items of input type 'itemType', stores
their pointers in an input array of pointers 'itemTList',
and stores the resulting number in input variable 'typeNum'.
If no instances are found, NULL is returned.
*/
Item **getItemsOfType(Item *itemList, size_t itemNum,
						const char itemType,
						Item **itemTList, size_t *typeNum);

/*
Reads item data from file in 'path',
saves it un an array of Item structures,
and returns a pointer to it.
*/
Item *readItems(const char *path, size_t *itemNum);


/*
Reads character data from file in 'path',
saves it un an array of Charac structures,
and returns a pointer to it.
*/
Charac *readCharac(const char *path, size_t *characNum);


/*
Reads race data from file in 'path',
saves it un an array of Race structures,
and returns a pointer to it.
*/
Race *readRaces(const char *path, size_t *raceNum);

/* Gather player info from file */
Charac *readPlayer(const char *path);

/* Save player info into file */
Charac *gameSave(Charac *player, const char* path);

void playerSetupPrintPoints(int *fScores);

int getAbMod(int abilityScore);

void playerSetupDistributePoints(int *finalScores, const int *scores,
									Race *race);

void playerSetupRollPoints(int *scores);

void playerSetupSaveScores(Charac *player, int *fScores);

Charac *setupPlayer(Race *races, size_t raceNum,
					Item *itemList, size_t itemNum);





#endif