

#include <stdio.h>

#include "include/utils.h"
#include "include/init.h"



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

	//print all characters
	printf("\n\n --- Characters:\n");
	for(size_t i=0; i<vsize(CHARACS); i++)
	{
		Item *cur = vat(CHARACS, i);
		printf("%s\n", cur->name);
	}

	game_free();

	//getchar();

quit:
	return err;
}