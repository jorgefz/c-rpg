#ifndef _INV_H
#define _INV_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/utils.h"
#include "include/init.h"


/*
String names corresponding to enum values
for equipment slots on inventory.
*/
const char *eqp_slot_names[EQP_NUM];



// FUNCTIONS
/* Initialize inventory */
vector *inv_init();

/* Adds an item to inventory */
vector *inv_add(vector *inv, Item *item);

/* Deep comparison of item structs */
size_t item_cmp(Item *a, Item *b);

/* Counts occurrences of an item in inventory */
size_t inv_count_item(vector *inv, Item *item);

/* Returns index of first occurrence of item in inventory */
size_t inv_where(vector *inv, Item *item, size_t begin);

/* Removes item from inventory */
vector *inv_remove(vector *inv, Item *item);

/* Removes all occurrences of an item from inventory */
vector *inv_rmall_item(vector *inv, Item *item);

/* Prints list of items in inventory with indices*/
void inv_print_list(vector *inv);

/* Prints list of equipped items with indices */
void inv_print_eqp(Charac *player);

/* Prints list of equipped and backpack items ^*/
void inv_print(Charac *player);

/* Unequips item at input slot ID */
Charac *inv_unequip(Charac *player, size_t slotId);

/* Equips item at input index in player inventory */
Charac *inv_equip(Charac *player, size_t itemId);


#endif

