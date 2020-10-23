#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#include "utils.h"
#include "vector.h"
#include "setup.h"



Item *searchItemByName(Item *itemList, size_t itemNum,
						const char *itemName)
{
	for(size_t i=0; i<itemNum; i++){
		if (strcmp(itemList[i].name, itemName) == 0)
			return &itemList[i];
	}
	return NULL;
}

Item *searchItemByID(Item *itemList, size_t itemNum, 
					size_t inputID)
{
	for(size_t i=0; i<itemNum; i++){
		if (itemList[i].ID == inputID)
			return &itemList[i];
	}
	return NULL;
}

Item **getItemsOfType(Item *itemList, size_t itemNum,
						const char *itemType,
						Item **itemTList, size_t *typeNum)
{
	*typeNum = 0;
	for(size_t i=0; i<itemNum; i++){
		if ( strcmp(itemList[i].type, itemType)==0 ){
			itemTList[*typeNum] = &itemList[i];
			*typeNum += 1;
		}
	}
	if (*typeNum == 0)
		return NULL;
	return itemTList;
}




Item *readItems(const char *path, size_t *itemNum)
{
	//Number of expected data columns in data, defined by Item struct
	size_t EXP_COLS = 4;

	size_t shape[2];
	size_t maxSize = 50;

	//Get data from txt
	char **itemData = GenFromTxt(path, &shape[0], maxSize, ';', '#');
	if(!itemData)
		return NULL;

	//Init item structs
	*itemNum = shape[0];
	Item *itemList = malloc(shape[0] * sizeof(Item));
	if(!itemList)
		return NULL;

	//Check expected columns
	if(shape[1] != EXP_COLS)
		return NULL;

	//Save item data in struct
	for(size_t i=0; i<shape[0]; i++)
	{
		//Slice item name if too long
		if(strlen(itemData[i*shape[1]]) > 50)
			strslc(itemData[i*shape[1]], 0, 95);
		//Get item name
		strcpy(itemList[i].name, itemData[i*shape[1]]);

		//Get item type
		strcpy(itemList[i].type, strNoSpaces(itemData[i*shape[1] + 1]) );
		
		// Save integer data into array
		int intData[EXP_COLS-2];
		for(size_t j=0; j<EXP_COLS-2; j++){
			if(!strtoint(&intData[j], itemData[i*shape[1] + (j+2)]) ){
				fprintf(stderr, " Error: unexpected format in %s\n", path);
				return NULL;
			}
		}

		//Copy into struct
		itemList[i].priceGold = intData[0];
		itemList[i].data = intData[1];
	}

	//Free data
	for(size_t i=0; i<shape[0]*shape[1]; i++)
		free(itemData[i]);
	free(itemData);

	return itemList;
}



// CHARACTERS
Charac *readCharac(const char *path, size_t *characNum)
{
	size_t EXP_COLS = 5;

	size_t shapeVal[2];
	size_t *shape;
	shape = shapeVal;
	size_t maxSize = 50;

	//Get data from txt
	char **chData = GenFromTxt(path, shape, maxSize, ';', '#');
	if(!chData){
		fprintf(stderr, " Error: %s not found\n", path);
		return NULL;
	}

	//Init charac structs
	*characNum = shape[0];
	Charac *chList = malloc(shape[0] * sizeof(Charac));
	if(!chList)
		return NULL;

	//Check expected columns
	if(shape[1] != EXP_COLS){
		fprintf(stderr, " Error: unexpected format in %s\n", path);
		return NULL;
	}

	//Save items into struct
	for(size_t i=0; i<shape[0]; i++)
	{
		//Get Charac name
		if(maxSize > 100)
			strslc(chData[i*shape[1]], 0, 99);
		strcpy(chList[i].name, chData[i*shape[1]]);
		
		// Save integer data into array
		int intData[EXP_COLS-1];
		for(size_t j=0; j<EXP_COLS-1; j++){
			if(!strtoint(&intData[j], chData[i*shape[1] + (j+1)]) ){
				fprintf(stderr, " Error: unexpected format in %s\n", path);
				return NULL;
			}
		}

		//Copy into struct
		chList[i].cons = intData[0];
		chList[i].strn = intData[1];
		chList[i].dext = intData[2];
		chList[i].intl = intData[3];
	}

	//Free GenFromTxt data
	for(size_t i=0; i<shape[0]*shape[1]; i++)
		free(chData[i]);
	free(chData);

	//Setting up active stats
	for(size_t i=0; i<shape[0]; i++)
	{
		chList[i].HP 	= chList[i].cons + 10;
		chList[i].maxHP = chList[i].HP;
		chList[i].SP 	= chList[i].strn + 10;
		chList[i].maxSP = chList[i].SP;
		chList[i].MP 	= chList[i].intl + 10;
		chList[i].maxMP = chList[i].MP;
	}

	return chList;
}





Race *readRaces(const char *path, size_t *raceNum)
{
	// Expected columns as Race Struct
	size_t EXP_COLS = 5;

	size_t shape[2];
	size_t maxSize = 50;

	//Get data from txt
	char **raceData = GenFromTxt(path, &shape[0], maxSize, ';', '#');
	if(!raceData){
		fprintf(stderr, " Error: %s not found\n", path);
		return NULL;
	}

	//Init race structs
	*raceNum = shape[0];
	Race *raceList = malloc(shape[0] * sizeof(Race));
	if(!raceList)
		return NULL;

	//Check expected columns
	if(shape[1] != EXP_COLS){
		fprintf(stderr, " Error: unexpected format in %s\n", path);
		return NULL;
	}

	//Save race data into struct
	for(size_t i=0; i<shape[0]; i++)
	{
		//Get Race name
		if(maxSize >= 100)
			strslc(raceData[i*shape[1] + 0], 0, 99);
		strcpy(raceList[i].name, raceData[i*shape[1]]);
		
		// Save integer data into array
		int intData[EXP_COLS-1];
		for(size_t j=0; j<EXP_COLS-1; j++){
			if(!strtoint(&intData[j], raceData[i*shape[1] + (j+1)]) ){
				fprintf(stderr, " Error: unexpected format in %s\n", path);
				return NULL;
			}
		}

		//Copy into struct
		raceList[i].cons = intData[0];
		raceList[i].strn = intData[1];
		raceList[i].dext = intData[2];
		raceList[i].intl = intData[3];
	}

	//Free data
	for(size_t i=0; i<shape[0]*shape[1]; i++)
		free(raceData[i]);
	free(raceData);

	return raceList;
}



/*
Read a saved game
*/

Charac *readPlayer(const char *path)
{
	size_t EXP_COLS = 5;

	size_t shapeVal[2];
	size_t *shape;
	shape = shapeVal;
	size_t maxSize = 50;

	//Get data from txt
	char **mData = GenFromTxt(path, shape, maxSize, ';', '#');
	if(!mData){
		fprintf(stderr, " Error: monsters file not found\n");
		return NULL;
	}

	//Init charac structs
	Charac *mList = malloc(shape[0] * sizeof(Charac));
	if(!mList)
		return NULL;

	//Check expected columns
	if(shape[1] != EXP_COLS){
		fprintf(stderr, " Error: unexpected formatting in monsters file\n");
		return NULL;
	}

	//Save items into struct
	for(size_t i=0; i<shape[0]; i++)
	{
		//Name, CON, STR, DEX, INT
		//Name
		if(maxSize > 100)
			strslc(mData[i*shape[1] + 0], 0, 99);
		strcpy(mList[i].name, mData[i*shape[1] + 0]);

		//Constitution Modifier
		int cons;
		int *retCons = strtoint(&cons, mData[i*shape[1] + 1]);
		if(!retCons){
			fprintf(stderr, " Error: unexpected formatting in monsters file\n");
			return NULL;
		}
		mList[i].cons = cons;
		
		//Strength Modifier
		int strn;
		int *retStrn = strtoint(&strn, mData[i*shape[1] + 2]);
		if(!retStrn){
			fprintf(stderr, " Error: unexpected formatting in monsters file\n");
			return NULL;
		}
		mList[i].strn = strn;

		//Dexterity Modifier
		int dext;
		int *retDext = strtoint(&dext, mData[i*shape[1] + 3]);
		if(!retDext){
			fprintf(stderr, " Error: unexpected formatting in monsters file\n");
			return NULL;
		}
		mList[i].dext = dext;

		//Intelligence Modifier
		int intl;
		int *retIntl = strtoint(&intl, mData[i*shape[1] + 4]);
		if(!retIntl){
			fprintf(stderr, " Error: unexpected formatting in monsters file\n");
			return NULL;
		}
		mList[i].intl = intl;

	}

	//Free data
	for(size_t i=0; i<shape[0]*shape[1]; i++)
		free(mData[i]);
	free(mData);

	return mList;
}




Charac *gameSave(Charac *player, const char* path)
{

	// Open file
	FILE *fdata;
    fdata = fopen(path, "w");
    if(!fdata){
    	fprintf(stderr, " Error: '%s' file could not be created\n", path);
    	return (NULL);
    }

    fprintf(fdata, "# Game save ");
    fprintf(fdata, "# Name; Race; HP; maxHP; MP; maxMP; CON; STR; DEX; INT\n");
    fprintf(fdata, "%s;%s;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\n",
    	player->name, player->race, player->HP, player->maxHP,
    	player->SP, player->maxSP, player->MP, player->maxMP,
    	player->cons, player->strn, player->dext, player->intl);
    fclose(fdata);
    return player;
}



//			PLAYER SETUP

void playerSetupPrintPoints(int *fScores)
{
	printf(" CON %d [%+d]\n", fScores[0], getAbMod(fScores[0]));
	printf(" STR %d [%+d]\n", fScores[1], getAbMod(fScores[1]));
	printf(" DEX %d [%+d]\n", fScores[2], getAbMod(fScores[2]));
	printf(" INT %d [%+d]\n", fScores[3], getAbMod(fScores[3]));
}

/* Calculates Ability Modifier */
int getAbMod(int abilityScore)
{
	float mod = (abilityScore - 10)/2;
	int iMod = (int)mod>=0? (int)(mod+0.5) : (int)(mod-0.5);
	return iMod;
}


void playerSetupDistributePoints(int *finalScores, const int *scores,
									Race *race)
{

	const char *abNames[4];
	abNames[0] = "Constitution";
	abNames[1] = "Strength";
	abNames[2] = "Dexterity";
	abNames[3] = "Intelligence";

	//Saves whether score has already been chosen (1) or not (0)
	int chosen[4] = {0, 0, 0, 0};
	//Store racial bonuses
	int racialBonus[4] = {race->cons, race->strn, race->dext, race->intl};


	//Distribute points
	int i = 0;
	while(i < 4){

		clear();
		printf(" ---------- Character Creation ----------\n");
		printf(" What score should your %s have?\n", abNames[i]);

		//Saves indices of count loop
		size_t scoreInd[4];
		int count = 0;
		
		//Loop over possible scores
		for(size_t j=0; j<4; j++){
			//Skip if score already chosen
			if(chosen[j] == 1)
				continue;
			printf(" %d) %d [%+d]\n", count+1, scores[j], getAbMod(scores[j]));
			scoreInd[count] = j;
			count++;
		}
		
		//Print racial bonus
		if(racialBonus[i] != 0)
			printf(" Note: %s bonus %+d\n", race->name, racialBonus[i]);
		
		//Choose
		int option = getint(-1) - 1;
		if(option < 0 || option >= count)
			continue;
		
		chosen[scoreInd[option]] = 1;
		finalScores[i] = scores[scoreInd[option]] + racialBonus[i];
		
		i++;
	}

	return;
}


/*
	Randomizes scores and returns values.
	Input scores must have size 4
*/
void playerSetupRollPoints(int *scores)
{

	for(size_t i=0; i<4; i++)
	{
		//Roll 4d6
		int rolls[4] = {rolld6(), rolld6(), rolld6(), rolld6()};

		//Find lowest
		size_t imin = intimin(&rolls[0], 4);

		//Sum discarding lowest
		int sum = 0;
		for(size_t j=0; j<4; j++){
			if (j != imin)
				sum += rolls[j];
		}

		//Save value
		scores[i] = sum;
	}

	return;
}



void playerSetupSaveScores(Charac *player, int *fScores)
{
	//Save ability scores
	player->cons = fScores[0];
	player->strn = fScores[1];
	player->dext = fScores[2];
	player->intl = fScores[3];

	//Calculate active stats
	int maxHitDice = 8;
	player->HP = getAbMod(player->cons) + maxHitDice;
	player->maxHP = player->HP; 
	player->SP = getAbMod(player->strn) + maxHitDice;
	player->maxSP = player->SP;
	player->MP = getAbMod(player->intl) + maxHitDice;
	player->maxMP = player->MP;
	return;
}


// RE-ROLL ABILITY SCORES




