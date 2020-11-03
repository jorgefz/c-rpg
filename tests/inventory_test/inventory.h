#ifndef _INV_HEADER_H
#define _INV_HEADER_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"



typedef struct itemStruct{
	int ID;
	char name[100];
	int price;
	/* Type: weapon, armor, etc */
	char type[10];
	/* Equipt type */
	int eqpType;

	/* Generic data (e.g. damage if weapon) */
	int data;
} Item;



// CHARACTERS

typedef enum EquipSlotEnum{
	EQP_HAND1,
	EQP_HAND2,
	EQP_HEAD,
	EQP_CHEST,
	EQP_ARMS,
	EQP_FEET,
	EQP_RING,
	EQP_NECKL,
	EQP_OTHER,
	EQP_NUM,	// Number of equip enum values
	EQP_NONE
} EquipSlot;


typedef struct charStruct{
	int id;
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
	Item *eqpSlots[8];
	// Check enum above

	//Item Inventory
	vector *inv;
	//Magic
	vector *magic;
} Charac;




/*
item type
	'melee' 	: swords, axes, etc
	'ranged'	: bows, throwing knives, etc
	'ammo'		: arrows
	'cons'		: consumables, food, potions, etc
	'a-head'	: head armor
	'a-chest'	:
	'a-hands'	:
	'a-feet'	:
	'a-ring'	:
*/


// GLOBAL VARIABLES
char *eqpSlotCodes[8];


char *eqpSlotNames[8];

size_t eqpSlotNum;

/*
Compares two item structs
and returns 0 if identical.
*/
size_t cmpItem(Item *a, Item *b);

	
/*
Searches for instances of a given item in
a vector inventory from a given index.
Returns the index of the first instance if found,
and the size of the inventory otherwise.
*/
size_t whereInv(vector *inv, Item *item, size_t begin);


/*
Adds a new item to the inventory, and increases its size by 1.
If there's no more space in inventory, NULL is returned. 
*/
vector * addToInv(vector *inv, Item *item);

/*
Given an input item, it searches for instances of it on the inventory.
The first instance is removed and the inventory size
is reduced by one.
If the input index is already empty, NULL is returned. 
*/
vector * delFromInv(vector *inv, Item *item);


void printInvItems(vector *inv);

void printEquippedItems(Charac *player);

void printPlayerInv(Charac *player);

Charac *storeEquipItem(Charac *player, size_t slotId);

Charac *equipItem(Charac *player, size_t itemId);

void menuInventory(Charac *player);


#endif

