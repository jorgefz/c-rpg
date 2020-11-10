#ifndef _INIT_H
#define _INIT_H 1

/*

	INIT.H

	Contains basic game structures and functions,
	as well as game initialization and freeing:

	Item and character structures.


	TO-DO:
		- Optimize file reading functions
		- 

*/


#include <stdio.h>
#include <stdlib.h>

#include "utils.h"



/* ================ MACROS ================ */
#define STR_LINE_MAX 100

/*
Expected number of data columns to find reading items data file.
This corresponds to: ID, name, type, equip type, price, data, ...

*/
#define ITEM_DATA_COLS 6

#define CHARAC_DATA_COLS 7


// Max number of players on their team
#define MAX_PLAYERS 5

#define check_quit if(err != NO_ERROR){goto quit;}



/* ================ STRUCTURES ================ */


// ERRORS

typedef enum error_enum{
	NO_ERROR = 0,
	MEM_ALLOC_ERROR,
	FILE_ERROR,
	UNKNOWN_ERROR
} Error;


// ITEMS

typedef enum ItemTypeEnum{
	ITEM_WEAPON = 0,
	ITEM_ARMOR,
	ITEM_FOOD,
	ITEM_AMMO,
	ITEM_POTION,
	ITEM_MISC,
	ITEM_TYPE_NUM
} ItemType;


typedef enum EquipSlotEnum{
	EQP_HAND1 = 0,	// Main hand, weapons
	EQP_HAND2,		// Secondary hand, shields, arrows, etc.
	EQP_HEAD,
	EQP_CHEST,
	EQP_ARMS,
	EQP_FEET,
	EQP_RING,
	EQP_NECKL,
	EQP_OTHER,
	EQP_NONE,		// Non-equippable
	EQP_NUM,		// Number of equip enum values
} EquipSlot;



typedef struct itemStruct{
	int id;
	char name[STR_LINE_MAX];
	/* Price in gold */
	int price;
	/* Item type as defined by enum */
	int type;
	/* Equipment slot in which it fits if equippable*/
	int eqp_type;
	/* Generic data (e.g. damage if weapon) */
	int data;
} Item;


// CHARACTERS


typedef enum charac_type{
	CHARAC_PLAYER = 0,
	CHARAC_MONSTER,
	CHARAC_RACE,
	CHARAC_NPC,

} CharacType;

typedef struct characStruct{
	int id;
	char name[STR_LINE_MAX];
	char race[STR_LINE_MAX];
	int type;
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
	Item *eqpSlots[EQP_NUM];
	//Item Inventory
	vector *inv;
	//Magic
	vector *magic;
} Charac;


/* ========== GLOBAL VARIABLES ========== */

// Logging
FILE *LOG;
extern const char *LOGFILE;

vector *ITEMS;
vector *CHARACS;
vector *PLAYERS;

// Equipment Slot Names, indices corresponding to enum values
const char *eqpSlotNames[EQP_NUM];

// Item type names, indices corresponding to enum values
const char *itemTypeNames[ITEM_TYPE_NUM];

// Current number of players in team
extern int num_players;


// Data reading directories
extern const char *ITEMS_TXT;
extern const char *CHARACS_TXT;
// Save game location
extern const char *SAVE_DIR;


/* ============= FUNCTION DECLARATIONS ============ */

//		INITIALIZATION

/* Logs game messages onto file */
void log_msg(char *msg, ...);

/* Opens log file */
Error log_init();


/* Initalises and allocates game objects */
Error game_init();

/* Frees every game object */
void game_free();


//		READ TXT

//	Items
//Error item_read_data(vector *v, const char *path);
Error item_read_data();
// Races
Error charac_read_data();

//	Charac
// characReadTxt

// General
Error game_read_data();


//		INDEXING

//	=== Items
Item *item_search_name(vector *v, const char *lookup_name);
Item *item_search_id(vector *v, int lookup_id);


//	=== Charac
vector *charac_search_type(int intype);
vector *charac_get_races();
// characSearchName
// characSearchId


#endif