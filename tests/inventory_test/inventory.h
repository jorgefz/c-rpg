#ifndef _INV_HEADER_H
#define _INV_HEADER_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"



#define MAX_STR_NAME 100


typedef struct itemStruct{
	// Unique for each item
	int id;
	char name[MAX_STR_NAME];
	int price;
	/* Item type as defined by enum */
	int type;
	/* Equipt type */
	int eqpType;

	/* Generic data (e.g. damage if weapon) */
	int data;
} Item;



typedef enum ItemTypeEnum{
	IT_WEAPON,
	IT_ARMOR,

} ItemType;



// CHARACTERS

typedef enum EquipSlotEnum{
	EQP_HAND1=0,
	EQP_HAND2,
	EQP_HEAD,
	EQP_CHEST,
	EQP_ARMS,
	EQP_FEET,
	EQP_RING,
	EQP_NECKL,
	EQP_OTHER,
	EQP_NUM,	// Number of equip enum values
	EQP_NONE	// Non-equippable
} EquipSlot;


typedef struct charStruct{
	int id;
	char name[MAX_STR_NAME];
	char race[MAX_STR_NAME];
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
	// Check enum above

	//Item Inventory
	vector *inv;
	//Magic
	vector *magic;
} Charac;





// GLOBAL VARIABLES
const char *eqpSlotNames[EQP_NONE+1];



// FUNCTIONS
/* Deep comparison of item structs */
size_t item_cmp(Item *a, Item *b);

/* Counts occurrences of an item in inventory */
size_t inv_count_item(vector *inv, Item *item);

/* Returns index of first occurrence of item in inventory */
size_t inv_where(vector *inv, Item *item, size_t begin);

/* Removes all occurrences of an item from inventory */
vector *inv_rmall_item(vector *inv, Item *item);

/* Initialize inventory */
vector *inv_init();

/* Adds an item to inventory */
vector *inv_add(vector *inv, Item *item);

/* Removes item from inventory */
vector *inv_remove(vector *inv, Item *item);

void inv_print_list(vector *inv);

void inv_print_eqp(Charac *player);

void inv_print(Charac *player);

Charac *inv_unequip(Charac *player, size_t slotId);

Charac *inv_equip(Charac *player, size_t itemId);


#endif

