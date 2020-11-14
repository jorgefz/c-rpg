

#include <stdio.h>

#include "include/utils.h"
#include "include/init.h"
#include "include/inventory.h"



int main()
{
	Error err = NO_ERROR;

	err = game_init();
	check_quit;

	err = game_read_data();
	check_quit;

	//print all items
	printf("\n\n --- Items:\n");
	for(size_t i=0; i<vsize(ITEMS); i++)
	{
		Item *cur = vat(ITEMS, i);
		printf("%s\n", cur->name);
	}

	printf("\n\n Item Attributes...\n");
	for(size_t i=0; i<vsize(ITEMS); i++)
	{
		Item *cur = vat(ITEMS, i);
		printf("   >%s: %d\n", item_get_attrib_name(cur->type, 0), cur->attrib[0]);
		printf("   >%s: %d\n", item_get_attrib_name(cur->type, 1), cur->attrib[1]);
	}

	//print all characters
	printf("\n\n --- Characters:\n");
	for(size_t i=0; i<vsize(CHARACS); i++)
	{
		Item *cur = vat(CHARACS, i);
		printf("%s\n", cur->name);
	}

	//retrieve races
	vector *races = charac_get_races();
	printf("\n\n --- Races:\n");
	for(size_t i=0; i<vsize(races); i++)
	{
		Charac *cur = *(Charac**)vat(races, i);
		printf("%s\n", cur->name);
	}
	vfree(races);

	getchar();





	// Inventory tests
	// Creating player
	Charac player = {.name="Player One"};
	player.inv = inv_init();
	if(!player.inv){
		printf("DEBUG: failed to initialize inventory\n");
		return 1;
	}
	for(size_t i=0; i<EQP_NUM-1; i++)
	{
		player.eqp_slots[i] = NULL;
	}

	printf("Player inventory should be empty...\n");
	inv_print(&player);

	getchar();
	printf("\n\n\n");

	inv_add(player.inv, vat(ITEMS,0));
	inv_add(player.inv, vat(ITEMS,1));
	inv_add(player.inv, vat(ITEMS,2));
	inv_add(player.inv, vat(ITEMS,3));
	inv_print(&player);

	getchar();
	printf("\n\n\n");

	inv_remove(player.inv, vat(ITEMS,2));
	inv_print(&player);

	getchar();
	printf("\n\n\n");

	inv_equip(&player, inv_where(player.inv, vat(ITEMS,3), 0) );
	inv_equip(&player, inv_where(player.inv, vat(ITEMS,0), 0) );
	inv_print(&player);

	getchar();
	printf("\n\n\n");
	inv_add(player.inv, vat(ITEMS,8));
	inv_add(player.inv, vat(ITEMS,9));
	inv_add(player.inv, vat(ITEMS,10));
	inv_add(player.inv, vat(ITEMS,11));

	inv_equip(&player, inv_where(player.inv, vat(ITEMS,8), 0) );
	inv_equip(&player, inv_where(player.inv, vat(ITEMS,9), 0) );
	inv_equip(&player, inv_where(player.inv, vat(ITEMS,10), 0) );
	inv_equip(&player, inv_where(player.inv, vat(ITEMS,11), 0) );
	inv_print(&player);
	
	getchar();

	vfree(player.inv);

quit:
	game_free(err);
	return err;
}