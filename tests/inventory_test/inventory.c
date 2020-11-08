

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
const char *eqpSlotNames[] = {
				"Main Hand",
				"Secondary Hand",
				"Head",
				"Chest",
				"Arms",
				"Feet",
				"Ring",
				"Necklace",
				"Other",
				"",			// For equip-num
				"Non-equipable"
				};



/*
NOTE: get pointer to item from inventory: Item *i = *(Item**)vat(inv, 0)
NOTE: get direct memory to item from inventory: Item i = **(Item**)vat(inv, 0)
*/

/*
Compares two item structs,
returns 0 if identical
and other value if not.
*/
size_t item_cmp(Item *a, Item *b)
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
Given an input item,
counts the number of identical items on an inventory.
*/
size_t inv_count_item(vector *inv, Item *item)
{
	size_t num = 0;
	for(size_t i = 0; i<vsize(inv); i++){
		Item *curr = *(Item**)vat(inv, i);
		if (item->id == curr->id){
			num++;
		}
	}
	return num;
}


/*
Find if an item is present
on the inventory.
It returs the index of the first instance.
If not found, it returns the size of the inventory.
*/
size_t inv_where(vector *inv, Item *item, size_t begin)
{
	//Default value for return index of item
	size_t index = vsize(inv);

	for(size_t i = begin; i<vsize(inv); i++){
		Item *curr = *(Item**)vat(inv, i);
		if (item->id != curr->id){
			continue;
		}
		//Save index of first instance found
		index = i;
		break;
	}
	return index;
}



/*
Removes all instances of an item from an inventory.
*/
vector *inv_rmall_item(vector *inv, Item *item)
{
	size_t index = 0;
	while( (index = inv_where(inv, item, index)) != vsize(inv)){
		inv_remove(inv, item);
	}
	return inv;
}


/* Initializes inventory */
vector *inv_init()
{
	vector *inv = vnew(sizeof(Item *));
	return inv;
}


/*
Adds a new item to the inventory, and increases its size by 1.
If there's no more space in inventory, NULL is returned. 
*/
vector *inv_add(vector *inv, Item *item)
{
	if(!item){
		return NULL;
	}

	/* Inventory stores pointers to items loaded in memory */
	vector *ret = vinsert(inv, 0, &item);
	if(!ret){
		printf("Failed to add item!\n");
		return NULL;
	}

	printf(" Added %s to inventory!\n", item->name);
	
	return inv;
}


/*
Removes an input item from the inventory, if present.
*/
vector * inv_remove(vector *inv, Item *item)
{
	//Sanitizing input
	if (vsize(inv) == 0){
		printf(" Inventory is already empty!\n");
		return NULL;
	}

	
	//Find item in inventory
	size_t index = inv_where(inv, item, 0);
	if (index == vsize(inv)){
		printf(" You can't remove an item you don't have!\n");
		return NULL;
	}

	//Remove item
	vector *ret = vdelete(inv, index);
	if(!ret){
		printf("Error removing item!");
		return NULL;
	}

	printf(" Removed %s from inventory!\n", item->name);

	return inv;
}



/*
Prints inventory items as a simple list, including indices
*/
void inv_print_list(vector *inv)
{
	for(size_t i=0; i<vsize(inv); i++){
		Item *curr = *(Item**)vat(inv, i);
		printf(" %"SZ_FMT") %s (%u G)\n", i+1, curr->name, curr->price);
	}
}

//Print only equipped items, including indices
void inv_print_eqp(Charac *player)
{
	for(size_t i=0; i<EQP_NUM; i++){
		//Skip if empty
		if(!player->eqpSlots[i]){
			continue;
		}
		printf(" %"SZ_FMT") %s: %10s\n", i+1, eqpSlotNames[i], player->eqpSlots[i]->name);
	}
}

// Print whole player inventory
void inv_print(Charac *player)
{
	//Equipped items
	printf(" Equipped:\n");
	for(size_t i=0; i<EQP_NUM; i++){
		//Skip if empty
		if(!player->eqpSlots[i]){
			continue;
		}
		printf(" - %s: %s\n", eqpSlotNames[i], player->eqpSlots[i]->name);
	}

	//Backpack
	printf("\n");
	printf(" Backpack:\n");
	for(size_t i=0; i<vsize(player->inv); i++){
		// Player inventory stores pointers (Item*) to where items are saved in memory.
		Item *curr = *(Item**)vat(player->inv, i);
		printf(" - %s (%u G)\n", curr->name, curr->price);
	}
}

/*
Moves item from character equip list to its inventory */
Charac *inv_unequip(Charac *player, size_t slotId)
{
	if( player->eqpSlots[slotId] == NULL ){
		return NULL;
	}

	inv_add(player->inv, player->eqpSlots[slotId]);
	player->eqpSlots[slotId] = NULL;

	return player;
}

//Equip item
Charac *inv_equip(Charac *player, size_t itemId)
{
	Item *item = *(Item**)vat(player->inv, itemId);
	if(!item){
		return NULL;
	}

	//Check if input item is allowed equip type
	size_t slotId = item->type;
	printf(" Equipping %s on slot %d (%s)\n", item->name, (int)slotId, eqpSlotNames[slotId]);

	if(slotId == EQP_NONE){
		printf(" You can't equip this item!\n");
		return NULL;
	}
	
	//If slot already full, move old item to inventory
	if( player->eqpSlots[slotId] ){
		inv_unequip(player, slotId);
	}
	
	//Equip & remove from normal inventory
	player->eqpSlots[slotId] = item;
	inv_remove(player->inv, item);

	return player;
}


//Inventory menu