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


size_t menuChooseInvItem(Charac *player)
{
	printf(" Backpack:\n");
	printInvItems(player->inv);
	int option = getint(vsize(player->inv)) - 1;
	if(option < 0 || option > (int)vsize(player->inv)-1)
		option = vsize(player->inv);
	return (size_t)option;
}

size_t menuChooseEquipItem(Charac *player)
{
	printf(" Equipment:\n");
	printEquippedItems(player);
	int slotId= getint(eqpSlotNum) - 1;
	if(slotId < 0 || slotId > (int)eqpSlotNum-1 || !player->eqpSlots[slotId])
		slotId = eqpSlotNum;
	return (size_t)slotId;	
}


void menuInventory(Charac *player)
{
	int menu = 0;
	while(menu == 0)
	{
		//Stats
		printf(" %s (%s)\n", player->name, player->race);
		printf(" HP: %d/%d ", player->HP, player->maxHP);
		printf(" SP: %d/%d ", player->SP, player->maxSP);
		printf(" MP: %d/%d \n", player->MP, player->maxMP);
		//Inventory
		printPlayerInv(player);

		//Equip menu
		printf(" 1) Equip Item\n");
		printf(" 2) De-equip item\n");
		printf(" 0) Back\n");
		char option = getAChar();
		switch(option)
		{
			case '1':
				/*Empty statement because reasons */;
				size_t equipId = menuChooseInvItem(player);
				if(equipId == vsize(player->inv))
					break;
				equipItem(player, equipId);
				break;
			case '2':
				/*Empty statement because reasons */;
				size_t slotId = menuChooseEquipItem(player);
				if(slotId == eqpSlotNum)
					break;
				storeEquipItem(player, slotId);
				break;
			case '3':
				menu = 1;
				break;
			default:
				break;
		}
	}
}


Charac *menuPlayerSetup(Race *races, size_t raceNum,
					Item *itemList, size_t itemNum)
{
	//Allocate players
	size_t maxPlayerNum = 6;

	Charac *playerList = malloc(sizeof(Charac)*maxPlayerNum);

	Charac *player;
	player = &playerList[0];

	//Initialise player inventory
	player->inv = vnew(sizeof(Item));
	player->gold = 100;

	int mainLoop = 0;
	while(mainLoop == 0)
	{
		//Race Loop
		int raceLoop = 0;
		while(raceLoop == 0)
		{
			clear();
			printf(" ---------- Character Creation ----------\n");
			printf(" -> What is your race?\n");
			for(size_t i=0; i<raceNum; i++){
				printf(" %Iu) %s 	", i+1, races[i].name);
				//Print non-zero stats
				if(races[i].cons != 0)
					printf(" CON+%d", races[i].cons);
				if(races[i].strn != 0)
					printf(" STR+%d", races[i].strn);
				if(races[i].dext != 0)
					printf(" DEX+%d", races[i].dext);
				if(races[i].intl != 0)
					printf(" INT+%d", races[i].intl);
				printf("\n");
			}
			char option = getAChar();
			int opInt = option - '0';
			if(opInt <= 0 || opInt > (int)raceNum)
				continue;
			raceLoop = opInt;
		}
		
		//Save race name
		raceLoop--;
		strcpy(player->race, races[raceLoop].name);

		//Get score rolls
		int scores[4];
		playerSetupRollPoints(&scores[0]);

		//Roll points loop
		while(1)
		{
			int finalScores[4];
			playerSetupDistributePoints(&finalScores[0], &scores[0], &races[raceLoop]);
			clear();
			printf(" ---------- Character Creation ----------\n");
			playerSetupPrintPoints(&finalScores[0]);
			printf(" 1) OK      0) Reset\n");
			char option = getAChar();
			if (option == '1'){
				playerSetupSaveScores(player, &finalScores[0]);
				break;
			}
			else
				continue;
		}
		
		//Charac name loop
		while(1)
		{
			clear();
			printf(" ---------- Character Creation ----------\n");
			printf(" -> What is your name?\n");
			char name[100];
			char *ret = getstr(&(name[0]), "> ");
			if(!ret)
				continue;
			printf(" 1) OK      0) Reset\n");
			char option = getAChar();
			if (option == '1'){
				strcpy(player->name, name);
				break;
			}
			else
				continue;
		}

		//Main Weapon loop
		while(1)
		{
			clear();
			printf(" ---------- Character Creation ----------\n");
			printf(" -> What is your starting inventory?\n");
			printf(" 1) Warrior \n");
			printf(" 2) Ranger \n");
			char option = getAChar();
			if (option == '1'){
				char *addItems[] = {"Sword", "Iron Helmet", "Iron Plate", "Iron Gauntlets", "Iron Boots"};
				for(size_t i=0; i<5; i++){
					addToInv(player->inv, searchItemByName(itemList, itemNum, addItems[i]));
					equipItem(player, 0);			
				}
			}
			else if (option == '2'){
				char *addItems[] = {"Bow", "Leather Helmet", "Leather Cuirass", "Leather Gloves", "Leather Boots"};
				for(size_t i=0; i<5; i++){
					addToInv(player->inv, searchItemByName(itemList, itemNum, addItems[i]));
					equipItem(player, 0);				
				}
			}
			else
				continue;
			break;
		}

		//Final Check
		while(1)
		{
			clear();
			printf(" ---------- Character Creation ----------\n");
			printf(" %s (%s)\n", player->name, player->race);
			printf(" HP: %d ", player->HP);
			printf(" SP: %d ", player->SP);
			printf(" MP: %d \n", player->MP);
			int scores[4] = {player->cons, player->strn, 
								player->dext, player->intl};
			playerSetupPrintPoints(&scores[0]);

			printf(" 1) OK      0) Cancel\n");
			char option = getAChar();
			if (option == '1' || option == '\0'){
				mainLoop = 1;
				break;
			}
			else if (option == '0')
				break;
			else
				continue;
		}

	}

	return playerList;
}
