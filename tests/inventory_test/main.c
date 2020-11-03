#include <stdio.h>
#include "utils.h"
#include "inventory.h"


int main()
{

	vector *items = vnew(sizeof(Item));
	Charac player;
	player.inv = vnew(sizeof(Item));

	//Item sword = {.name="Sword", .price=60};


	vfree(items);
	vfree(player.inv);


	return 0;
}