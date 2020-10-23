

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "vector.h"
#include "setup.h"
#include "inventory.h"


// GLOBAL VARIABLES
char *eqpSlotCodes[] = {
				"melee",
				"a-head",
				"a-chest",
				"a-hands", 
				"a-feet",
				"a-ring",
				"a-neckl",
				"a-other"
				};


char *eqpSlotNames[] = {
				"Main Hand",
				"Head",
				"Chest",
				"Hands",
				"Feet",
				"Ring",
				"Necklace",
				"Other"
				};

size_t eqpSlotNum = 8;


/*
Compares two item structs
and returns 0 if identical.
*/
size_t cmpItem(Item *a, Item *b)
{
	//Check if adresses are identical
	if(a == b)
		return 0;

	//Manual check
	if (!a || !b)
		return 1;
	if (strcmp(a->name, b->name) != 0)
		return 2;
	if (a->priceGold != b->priceGold)
		return 3;
	return 0;
}

	
/*
Searches for instances of a given item in
a vector inventory from a given index.
Returns the index of the first instance if found,
and the size of the inventory otherwise.
*/
size_t whereInv(vector *inv, Item *item, size_t begin)
{
	//Sanitize input beginning index
	if( begin >= vsize(inv))
		return vsize(inv);

	//Search for instances
	size_t index = vsize(inv);

	for(size_t i = begin; i<vsize(inv); i++){

		Item *curr = vat(inv, i);
		if (cmpItem(item, curr) != 0)
			continue;
		//Save index of first instance found
		index = i;
		break;
	}
	return index;
}


/*
Adds a new item to the inventory, and increases its size by 1.
If there's no more space in inventory, NULL is returned. 
*/
vector * addToInv(vector *inv, Item *item)
{
	if(!item)
		return NULL;
	//Diags
	if(!inv)
		printf("Inventory is null\n");
	printf("Inventory has size %Iu\n", vsize(inv));
	printf("Item has name %s\n", item->name);

	vector *ret = vinsert(inv, 0, item);
	if(!ret)
		printf("Failed\n");
	else
		printf("Item added\n");
	return inv;
}


/*
Given an input item, it searches for instances of it on the inventory.
The first instance is removed and the inventory size
is reduced by one.
If the input index is already empty, NULL is returned. 
*/
vector * delFromInv(vector *inv, Item *item)
{
	//Sanitizing input
	if (vsize(inv) == 0){
		printf(" Inventory is already empty!\n");
		return NULL;
	}

	
	//Find item in inventory
	size_t index = whereInv(inv, item, 0);
	if (index == vsize(inv)){
		printf(" You can't remove an item you don't have!\n");
		return NULL;
	}

	//Remove item
	vector *ret = vdelete(inv, index);
	if(!ret){
		printf("Error!");
		exit(0);
	}

	return inv;
}



/*
Prints inventory items as a simple list
*/
void printInvItems(vector *inv)
{
	for(size_t i=0; i<vsize(inv); i++){
		Item *curr = vat(inv, i);
		printf(" %Iu) %s (%u G)\n", i+1, curr->name, curr->priceGold);
	}
}

//Print only equipped items
void printEquippedItems(Charac *player)
{
	for(size_t i=0; i<eqpSlotNum; i++){
		//Skip if empty
		if(!player->eqpSlots[i])
			continue;
		printf(" %Iu) %s: %10s\n", i+1, eqpSlotNames[i], player->eqpSlots[i]->name);
	}
}

// Print whole player inventory
void printPlayerInv(Charac *player)
{
	//Equipped items
	printf(" Equipped:\n");
	for(size_t i=0; i<eqpSlotNum; i++){
		//Skip if empty
		if(!player->eqpSlots[i])
			continue;
		printf(" - %s: %s\n", eqpSlotNames[i], player->eqpSlots[i]->name);
	}

	//Backpack
	printf("\n");
	printf(" Backpack:\n");
	for(size_t i=0; i<vsize(player->inv); i++){
		Item *curr = vat(player->inv, i);
		printf(" - %s (%u G)\n", curr->name, curr->priceGold);
	}
}



Charac *storeEquipItem(Charac *player, size_t slotId)
{
	if(!player->eqpSlots[slotId])
		return NULL;

	addToInv(player->inv, player->eqpSlots[slotId]);
	player->eqpSlots[slotId] = NULL;

	return player;
}

//Equip item
Charac *equipItem(Charac *player, size_t itemId)
{
	Item *item = vat(player->inv, itemId);
	if(!item)
		return NULL;

	//Check if input item is allowed equip type
	size_t slotId = eqpSlotNum;
	for(size_t i=0; i<6; i++)
	{
		if(strcmp(eqpSlotCodes[i], item->type)==0){
			slotId = i;
			break;
		}
		
	}
	//Check for ranged case too (alt. for melee)
	if(strcmp(item->type, "ranged")==0)
		slotId = 0;

	if(slotId == eqpSlotNum){
		printf(" You can't equip this item!\n");
		return NULL;
	}

	//If slot already full, move old item to inventory
	storeEquipItem(player, slotId);
	
	//Equip
	player->eqpSlots[slotId] = item;

	//Remove from inventory
	delFromInv(player->inv, item);

	return player;
}


//Inventory menu