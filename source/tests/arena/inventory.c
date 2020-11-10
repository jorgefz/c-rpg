

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "setup.h"

/*
char type:
	Weapons
	wm - weapon melee
	wr - weapon ranged

	Armor
	ah - armor head
	ac - armor chest
	aa - armor arms
	al - armor legs
	af - armor feet

	Items
	ic - items consumables
	ia - items ammo
	ir - items resources
	io - items others

	Potions
	ph - Health potion
	ps - stamina potion
	pm - magicka potion
	


	Store as
	char primaryType = w, a, i
	char secType = m,r,h,c,a,l,f,p,o
*/




/*
Compares two item structs and returns 0 if identical.
*/
uint cmpItem(Item *a, Item *b)
{
	if (!a || !b)
		return 1;
	if (strcmp(a->name, b->name) != 0)
		return 2;
	if (a->priceGold != b->priceGold)
		return 3;
	return 0;
}


/*
Adds a new item to the inventory, and increases its size by 1.
If there's no more space in inventory, NULL is returned. 
*/
Item * addToInv(Item *item, Item *invList, size_t *invSize){
	if(!item || !invList)
		return NULL;
	if (*invSize == maxInvItems){
		printf(" Inventory full!\n");
		return NULL; 
	}
	invList[*invSize] = *item;
	*invSize = *invSize + 1;
	printf(" %s added to inventory\n", item->name);
	return invList;
}


/*
Given an input item, it searches for instances of it on the inventory.
The first instance is removed and the inventory size
is reduced by one.
If the input index is already empty, NULL is returned. 
*/
Item * delFromInv(Item *item, Item *invList, size_t *invSize){
	if (*invSize == 0){
		printf(" Inventory is already empty!\n");
		return NULL;
	}
	//Search for instances
	size_t index = maxInvItems;
	for(size_t i=0; i<*invSize; i++){
		if (cmpItem(item, &invList[i]) == 0){
			index = i;
			break;
		}
	}
	//No instances found
	if (index == maxInvItems){
		printf(" You can't remove an item you don't have!\n");
		return NULL;
	}
	//Remove item
	for(size_t i=index; i<*invSize; i++)
		invList[i] = invList[i+1];
	*invSize = *invSize - 1;
	return invList;
}

/*
Prints inventory item's names
*/
void printInv(Item *invList, size_t *invSize)
{
	for(size_t i=0; i<*invSize; i++)
		printf(" %Iu) %s (P%u)\n", i+1, invList[i].name, invList[i].priceGold);
	return;
}





void playerEquipItem(Charac *player, Item *itemEquip)
{
	if(itemEquip->type != 'w' || itemEquip->type != 'a'){
		printf(" You can't equip this item.\n");
		return,
	}

	// Equip Weapon
	if (itemEquip->type == 'w'){
		//Set main hand pointer to it
		//Remove from inventory
		return;
	}

	if (itemEquip->type == 'a')
	{
		//Switch between armour types
		switch(itemEquip->type2)
		{
			case 'h':
				//Set to head slot
				break;
			case 'c':
				//Set chest slot
				break;
			case 'a':
				//Set to arms slot
				break;
			case 'l':
				//Set to legs slot
				break;
			case 'f':
				//Set to feet slot
				break;
			default:
				printf(" You can't equip this item.\n");
				break;
		}
		//Remove from inventory
		return;
	}
}



void playerUseItem(Charac *player, Item *itemEquip)
{
	if(itemEquip->type == 'w' || itemEquip->type == 'a'){
		playerEquipItem(player, itemEquip)
		return,
	}

	// Equip Weapon
	if (itemEquip->type == 'w'){
		//Set main hand pointer to it
		//Remove from inventory
		return;
	}

	if (itemEquip->type == 'a')
	{
		//Switch between armour types
		switch(itemEquip->type2)
		{
			case 'h':
				//Set to head slot
				break;
			case 'c':
				//Set chest slot
				break;
			case 'a':
				//Set to arms slot
				break;
			case 'l':
				//Set to legs slot
				break;
			case 'f':
				//Set to feet slot
				break;
			default:
				printf(" You can't equip this item.\n");
				break;
		}
		//Remove from inventory
		return;
	}
}