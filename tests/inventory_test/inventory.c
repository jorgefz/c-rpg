

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "inventory.h"


// GLOBAL VARIABLES

/*
Names of character equipment slots.
Matches with values of EqpSlots enum.
*/
char *eqpSlotNames[] = {
				"Main Hand",
				"Secondary Hand",
				"Head",
				"Chest",
				"Arms",
				"Feet",
				"Ring",
				"Necklace",
				"Other",
				"(eqp-num)"
				"Non-equipable"
				};




/*
Compares two item structs,
returns 0 if identical
and other value if not.
*/
size_t cmpItem(Item *a, Item *b)
{
	//Check if adresses are identical
	if(a == b){
		return 0;
	}
	else if(a->id == b->id){
		return 0;
	}
	return 1;
}



/*
Counts the number of instances of
a given item in an inventory.
*/
size_t countInv(vector *inv, Item *item)
{
	size_t num = 0;
	for(size_t i = 0; i<vsize(inv); i++){
		Item *curr = vat(inv, i);
		if (cmpItem(item, curr) != 0)
			continue;
		//Save index of first instance found
		num++;
	}
	return num;
}


/*
Find if an item is present
on the inventory.
It returs the index of the first instance.
If not found, it returns the size of the inventory.
*/
size_t whereInv(vector *inv, Item *item, size_t begin)
{
	//Search for instances
	size_t inv_size = vsize(inv);
	size_t index = inv_size;

	//Sanitize input beginning index
	if( begin >= inv_size)
		return inv_size;

	for(size_t i = begin; i<inv_size; i++){
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
Removes all instances of an item from an inventory.
*/
vector *removeallInv(vector *inv, Item *item)
{
	size_t index;
	while( index = whereInv(inv, item) != vsize(inv)){
		delFromInv(inv, item);
	}
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
	printf("Inventory has size %"SZ_FMT"\n", vsize(inv));
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
		printf(" %"SZ_FMT") %s (%u G)\n", i+1, curr->name, curr->price);
	}
}

//Print only equipped items
void printEquippedItems(Charac *player)
{
	for(size_t i=0; i<EQP_NUM; i++){
		//Skip if empty
		if(!player->eqpSlots[i])
			continue;
		printf(" %"SZ_FMT") %s: %10s\n", i+1, eqpSlotNames[i], player->eqpSlots[i]->name);
	}
}

// Print whole player inventory
void printPlayerInv(Charac *player)
{
	//Equipped items
	printf(" Equipped:\n");
	for(size_t i=0; i<EQP_NUM; i++){
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
		printf(" - %s (%u G)\n", curr->name, curr->price);
	}
}



Charac *unequipItem(Charac *player, size_t slotId)
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
	size_t slotId = EQP_NUM;
	if(item->eqpType == -1){
		return NULL;
	}
	else{
		slotId = item->eqpType;
	}

	//Check for ranged case too (alt. for melee)
	if(strcmp(item->type, "ranged")==0)
		slotId = 0;

	if(slotId == EQP_NUM){
		printf(" You can't equip this item!\n");
		return NULL;
	}

	//If slot already full, move old item to inventory
	unequipItem(player, slotId);
	
	//Equip
	player->eqpSlots[slotId] = item;

	//Remove from inventory
	delFromInv(player->inv, item);

	return player;
}


//Inventory menu