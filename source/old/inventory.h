#ifndef _INV_HEADER_H
#define _INV_HEADER_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "vector.h"
#include "setup.h"


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

