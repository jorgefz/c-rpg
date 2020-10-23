#ifndef _INIT_H
#define _INIT_H 1

/*

	INIT.H

	Contains all game structures
	and initialisation functions.

*/


#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


/* ================ STRUCTURES ================ */


// ITEMS

typedef struct itemStruct{
	int ID;
	char name[100];
	int price;
	/* Type: weapon, armor, etc */
	char type[10];
	/* Generic data (e.g. damage if weapon) */
	int data;
} Item;



// CHARACTERS

typedef enum EquipSlotEnum{
	EQP_ATTACK,
	EQP_HEAD,
	EQP_CHEST,
	EQP_HANDS,
	EQP_FEET,
	EQP_RING,
	EQP_NECKL,
	EQP_OTHER
} EquipSlot;

typedef struct charStruct{
	int ID;
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
	Item eqpSlots[8];
	// Check enum above

	//Item Inventory
	vector *inv;
	//Magic
	vector *magic;
} Charac;




// GAME DATA

/* Stores every object in game */
typedef struct gameDataStruct
{
	//Structure vectors
	vector *items;
	vector *races;
	vector *npc;
	vector *players;
	//Skills
	//Magic
	//Locations
	//etc

	//Variables
	int maxPlayers;	//Max players in player's team

} GameData;



// COMBAT DATA
/*
Stores temporal information about a character in combat
*/
typedef struct combatDataStruct
{
	// Pointer to character in question
	Charac *ch;
	// '1' if yes, '0' if enemy
	int isAlly;
	// Initiative value
	int init;
	// Index of intiative sorted array
	size_t indSort;
	// '1' yes, '0' no
	int isDead;
	// Check to skip turn if stunned, etc
	int isStun;
	// Counter for turns stunned
	int cStun;
} CombatData;




// LOCATIONS

// Individual shop-like locations
typedef struct shopStruct
{
	//Location name
	char name[20];
	//Name of owner or faction
	char owner[20];
	//Tye pof location: mine, etc
	char type[20];

	/* Flags */
	int dealsGeneric;
	int dealsWeapons;
	int dealsArmor;
	int dealsFood;
	int dealsPotions;

	/* Item selling info */
	int maxSellLevel;
	int minSellLevel;
	int sellNum;
	vector *sellInv;

} Shop;


typedef struct locationStruct
{
	char name[20];
	char description[100];
	// City, town, mine, etc
	char type[20];

	vector *shops;
	// General Goods - everything
	// Blacksmith - weapons & armour
	// Inn - food & sleep
	// etc

	// Other locations
	//Castles
	//Dungeons, Caves, Camps
} Loc;



/* ============= FUNCTION DECLARATIONS ============ */

//		INITIALIZATION

/* Initalises and allocates game objects */
GameData *initGame();

/* Frees every game object */
void freeGame(GameData *gd);


//		READ TXT

//	Items
vector *itemReadTxt(vector *v, const char *path);
// Races
vector *characRaceReadTxt(vector *v, const char *path);

//	Charac
// characReadTxt

// General
GameData *readGameData(GameData *gd);


//		INDEXING

//	=== Items

// Search by name, returns first instance,
// and NULL otherwise
Item *itemSearchName(vector *v, const char *lookupName);

// Search by ID, returns first instance,
// and NULL otherwise
Item *itemSearchID(vector *v, int lookupID);

// Searches for items of given type,
// stores them in vector and returns it.
vector *itemGetType(vector *v, const char *lookupType);

//	=== Charac
// characSearchName
// characSearchId


#endif