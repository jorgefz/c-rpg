#ifndef _MENU_HEADER_H
#define _MENU_HEADER_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utils.h"
#include "vector.h"
#include "setup.h"
#include "inventory.h"
#include "combat.h"

/*

	Stores game menus
		- Character Creation
		- Inventory Menu
*/

size_t menuChooseInvItem(Charac *player);

size_t menuChooseEquipItem(Charac *player);

void menuInventory(Charac *player);

Charac *menuPlayerSetup(Race *races, size_t raceNum,
					Item *itemList, size_t itemNum);


#endif