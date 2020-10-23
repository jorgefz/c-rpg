

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "vector.h"
#include "setup.h"
#include "inventory.h"



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
Prints inventory item's names
*/
void printInv(vector *inv)
{
	for(size_t i=0; i<vsize(inv); i++){
		Item *curr = vat(inv, i);
		printf(" %Iu) %s (%u G)\n", i+1, curr->name, curr->priceGold);
	}
}


/*
Adds a new item to the inventory, and increases its size by 1.
If there's no more space in inventory, NULL is returned. 
*/
vector * addToInv(vector *inv, Item *item)
{
	return vinsert(inv, vsize(inv), item);
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