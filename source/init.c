


#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "init.h"






//		INITIALIZATION

/* Initalises and allocates game objects */
GameData *initGame()
{
	// ALLOCATE STRUCTURE VECTORS
	//Init items
	vector *items = vnew(sizeof(Item));

	//Init player
	vector *players = vnew(sizeof(Charac));

	//Init characters
	vector *npc = vnew(sizeof(Charac));

	//Init races
	vector *races = vnew(sizeof(Charac));

	//Init game data
	GameData *gd = malloc(sizeof(GameData));
	if(!gd){
		fprintf(stderr, "Fatal Error: unable to allocate memory\n");
		exit(-1);
	}
	gd->items = items;
	gd->npc = npc;
	gd->players = players;

	// DEFINE VARIABLES
	gd->maxPlayers = 6;

	size_t bytes = sizeof(vector)*3 + sizeof(GameData);
	printf("Allocated %Iu bytes\n", bytes);

	return gd;
}


/* Frees every game object */
void freeGame(GameData *gd)
{
	//Free vectors
	vfree(gd->items);
	vfree(gd->races);
	vfree(gd->players);
	vfree(gd->npc);

	free(gd);
}



//______________________________________________
//		READ DATA FROM TXT
//______________________________________________


// Read Item Data from TXT
vector *itemReadTxt(vector *v, const char *path)
{
	
	//Number of expected data columns in data, defined by Item struct
	size_t EXP_COLS = 5;
	//Array stores number of rows and columns
	size_t shape[2];
	//Max size of each line in characters
	size_t maxSize = 50;

	//Get data from txt
	char **itemData = GenFromTxt(path, &shape[0], maxSize, ';', '#');
	if(!itemData){
		fprintf(stderr, " Error: could not read items.txt\n");
		exit(-1);
	}

	//Check expected columns
	if(shape[1] != EXP_COLS){
		fprintf(stderr, " Error: wrong formatting in items.txt\n");
		exit(-1);
	}

	//Save txt data in item vector
	for(size_t i=0; i<shape[0]; i++)
	{	
		//Slice name if too long
		if(strlen(itemData[i*shape[1]]+1) > 50)
			strslc(itemData[i*shape[1]+1], 0, 50);

		//Retrieve in order:
		// ID (int), name (string), type (string), price (int)
		Item temp;

		//Get ID
		if(!strtoint(&temp.ID, itemData[i*shape[1]])){
			fprintf(stderr, " Error: wrong formatting in items.txt\n");
			exit(-1);
		}

		//Get name
		strcpy(temp.name, strNoSpaces(itemData[i*shape[1] + 1]));

		//Get type
		strcpy(temp.type, strNoSpaces(itemData[i*shape[1] + 2]) );

		//Get price
		if(!strtoint(&temp.price, itemData[i*shape[1] + 3])){
			fprintf(stderr, " Error: wrong formatting in items.txt\n");
			exit(-1);
		}

		//Get data
		if(!strtoint(&temp.data, itemData[i*shape[1] + 4])){
			fprintf(stderr, " Error: wrong formatting in items.txt\n");
			exit(-1);
		}

		//Save new item in vector
		vinsert(v, vsize(v), &temp);
	}

	//Free GenFromTxt return array
	for(size_t i=0; i<shape[0]*shape[1]; i++)
		free(itemData[i]);
	free(itemData);

	return v;
}


// Read Race Data from TXT
vector *characRaceReadTxt(vector *v, const char *path)
{
	
	//Number of expected data columns in data, defined by relevant struct
	size_t EXP_COLS = 6;
	//Array stores number of rows and columns
	size_t shape[2];
	//Max size of each line in characters
	size_t maxSize = 100;

	//Get data from txt
	char **raceData = GenFromTxt(path, &shape[0], maxSize, ';', '#');
	if(!raceData){
		fprintf(stderr, " Error: could not read races.txt\n");
		exit(-1);
	}

	//Check expected columns
	if(shape[1] != EXP_COLS){
		fprintf(stderr, " Error: wrong formatting in races.txt\n");
		exit(-1);
	}

	//Save txt data in item vector
	for(size_t i=0; i<shape[0]; i++)
	{	
		//Slice name if too long
		if(strlen(raceData[i*shape[1]]+1) > 99)
			strslc(raceData[i*shape[1]+1], 0, 99);

		//Retrieve in order:
		// Name (string), playable (int), CON, STR, DEX, INT (ints)
		Charac temp;

		//Get name
		strcpy(temp.name, strNoSpaces(raceData[i*shape[1]]));

		//Get playable flag
		if(!strtoint(&temp.ID, raceData[i*shape[1] + 1])){
			fprintf(stderr, " Error: wrong formatting in races.txt\n");
			exit(-1);
		}

		//Get CON
		if(!strtoint(&temp.cons, raceData[i*shape[1] + 2])){
			fprintf(stderr, " Error: wrong formatting in races.txt\n");
			exit(-1);
		}

		//Get STR
		if(!strtoint(&temp.strn, raceData[i*shape[1] + 3])){
			fprintf(stderr, " Error: wrong formatting in races.txt\n");
			exit(-1);
		}

		//Get DEX
		if(!strtoint(&temp.dext, raceData[i*shape[1] + 4])){
			fprintf(stderr, " Error: wrong formatting in races.txt\n");
			exit(-1);
		}

		//Get INT
		if(!strtoint(&temp.intl, raceData[i*shape[1] + 5])){
			fprintf(stderr, " Error: wrong formatting in races.txt\n");
			exit(-1);
		}


		//Save new item in vector
		vinsert(v, vsize(v), &temp);
	}

	//Free GenFromTxt return array
	for(size_t i=0; i<shape[0]*shape[1]; i++)
		free(raceData[i]);
	free(raceData);

	return v;
}


// Read Character Data from TXT



// Wrap fucntion to read all data
GameData *readGameData(GameData *gd)
{
	
	// Items
	itemReadTxt(gd->items, "data/items.txt");
	// Races
	characRaceReadTxt(gd->races, "data/races.txt");
	//Characs
	//etc

	return gd;
}






//______________________________________________
//		STRUCT INDEXING
//______________________________________________

//	ITEMS
// Search by name, returns first instance,
// and NULL otherwise
Item *itemSearchName(vector *v, const char *lookupName)
{
	for(size_t i=0; i<vsize(v); i++)
	{
		Item *temp = vat(v, i);
		if (strcmp(temp->name, lookupName) == 0)
			return temp;
	}
	return NULL;
}

// Search by ID, returns first instance,
// and NULL otherwise
Item *itemSearchID(vector *v, int lookupID)
{
	for(size_t i=0; i<vsize(v); i++)
	{
		Item *temp = vat(v, i);
		if (temp->ID == lookupID)
			return temp;
	}
	return NULL;
}

// Searches for items of given type,
// stores them in vector and returns it.
vector *itemGetType(vector *v, const char *lookupType)
{
	vector *t = vnew(sizeof(Item));
	for(size_t i=0; i<vsize(v); i++)
	{
		Item *temp = vat(v, i);
		if(strcmp(temp->type, lookupType)==0)
			vinsert(t, vsize(t), temp);
	}

	if(vsize(t)==0)
	{
		vfree(t);
		return NULL;
	}
	return t;
}

// CHARACS
// Search by name, returns first instance,
// and NULL otherwise
Charac *characSearchName(vector *v, const char *lookupName)
{
	for(size_t i=0; i<vsize(v); i++)
	{
		Charac *temp = vat(v, i);
		if (strcmp(temp->name, lookupName) == 0)
			return temp;
	}
	return NULL;
}

// Search by ID, returns first instance,
// and NULL otherwise
Charac *CharacSearchID(vector *v, int lookupID)
{
	for(size_t i=0; i<vsize(v); i++)
	{
		Charac *temp = vat(v, i);
		if (temp->ID == lookupID)
			return temp;
	}
	return NULL;
}

