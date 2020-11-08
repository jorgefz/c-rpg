#include <stdio.h>
#include "utils.h"
#include "inventory.h"



int main()
{
	printf("\n\n\n");

	/*
	This vector stores all items in memory.
	Other usages of items (inventories, etc),
	will point to memory locations on this vector.
	That means it must not be tampered with!
	*/
	vector *items = vnew(sizeof(Item));
	Item sword = {.id=0, .name="Sword", .price=60, .type=EQP_HAND1 };
	Item hpotion = {.id=1, .name="Health Potion", .price=20, .type=EQP_NONE };
	Item axe = {.id=2, .name="Axe", .price=50, .type=EQP_HAND1 };
	Item chestplate = {.id=3, .name="Steel Chestplate", .price=80, .type=EQP_CHEST };

	vinsert(items, vsize(items), &sword);
	vinsert(items, vsize(items), &hpotion);
	vinsert(items, vsize(items), &axe);
	vinsert(items, vsize(items), &chestplate);

	printf(" All items:\n");
	for(size_t i=0; i<vsize(items); i++)
	{
		Item *cur = vat(items, i);
		printf(" %"SZ_FMT") %s (%d)\n", i, cur->name, cur->price );
	}

	getchar();
	printf("\n\n\n");

	//Debug: print item types
	printf(" Equip types:\n");
	for(int i=0; i<4; i++)
	{
		Item *cur = vat(items,i);
		printf("%s: %s (%d)\n", cur->name, eqpSlotNames[cur->type], cur->type);
	}

	getchar();
	printf("\n\n\n");


	
	// Creating player
	Charac player = {.name="Player One"};
	player.inv = inv_init();
	if(!player.inv){
		printf("DEBUG: failed to initialize inventory\n");
		return 1;
	}
	for(size_t i=0; i<EQP_NUM; i++)
	{
		player.eqpSlots[i] = NULL;
	}

	printf("Player inventory should be empty...\n");
	inv_print(&player);

	getchar();
	printf("\n\n\n");

	inv_add(player.inv, vat(items,0));
	inv_add(player.inv, vat(items,1));
	inv_add(player.inv, vat(items,2));
	inv_add(player.inv, vat(items,3));
	inv_print(&player);

	getchar();
	printf("\n\n\n");

	inv_remove(player.inv, vat(items,2));
	inv_print(&player);

	getchar();
	printf("\n\n\n");

	inv_equip(&player, inv_where(player.inv, vat(items,3), 0) );
	inv_equip(&player, inv_where(player.inv, vat(items,0), 0) );
	inv_print(&player);


	vfree(items);
	vfree(player.inv);


	return 0;
}