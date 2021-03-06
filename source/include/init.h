#ifndef _INIT_H
#define _INIT_H 1

/*

	INIT.H

	Contains basic game structures and functions,
	as well as game initialization and freeing:

	init.h
		Error enum

		Item struct
		Equip Slot struct
		Item type enum

		Charac struct
		Charac type enum

	init.c
		log_msg
		log_init

		game_init
		game_free

		item_read_data
		charac_read_data
		game_read_data

		charac_search_type
		charac_get_races
		charac_search_name
		charac_search_id
	
		item_search_type
		item_search_name
		item_search_id

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
#define ITEM_DATA_COLS 7

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
	ITEM_WEAP_RANGED,
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

/*
Number of usable equipment slots in player inventory.
Ignores EQP_NONE.
*/
#define EQP_SLOTS EQP_NUM-1



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

	// Substitute for data
	int attrib[2];
} Item;

/*
Enum for item attributes.
Gives names to the nameless attributes for all item types.
Get using item->attrib[ATTRIBUTE_NAME] or with the function.
*/
typedef enum ItemTypeAttribEnum
{
	WEAP_MELEE_DMG=0,
	WEAP_MELEE_WEIGHT=1,

	ARMOR_DEF=0,
	ARMOR_WEIGHT=1,

} ItemAttrib;


/*
String names of the item attributes.
attrib_names[ItemType*2 + ItemAttrib]
*/
extern const char *item_attrib_names[][2];




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
	Item *eqp_slots[EQP_SLOTS];
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
void game_free(int err);


//		DATA READING
Error item_read_data();
Error charac_read_data();
Error game_read_data();


//		INDEXING
//	=== Items
vector *item_search_type(int intype);
Item *item_search_name(const char *lookup_name);
Item *item_search_id(int lookup_id);
const char *item_get_attrib_name(int type, int attrib);
//	=== Charac
vector *charac_search_type(int intype);
vector *charac_get_races();
Charac *charac_search_name(const char *name);
Charac *charac_search_id(int id);
void charac_free();


#endif