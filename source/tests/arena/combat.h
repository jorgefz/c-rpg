#ifndef _COMBAT_HEADER_H
#define _COMBAT_HEADER_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utils.h"
#include "setup.h"


/*
Stores temporal information about a character in combat
*/
typedef struct combatDataStruct
{
	// Pointer to character in question
	Charac *ch;
	
	// '1' if yes, '0' if enemy
	int isAlly;

	// Initiative value
	int init;

	// Index of intiative sorted array
	size_t indSort;
	
	// '1' yes, '0' no
	int isDead;
	
	// Check to skip turn if stunned, etc
	int isStun;

	// Set to max number of turns stunned,
	// subtract 1 each turn
	int counterTurnStun;



} CombatData;



size_t getTeamNum(CombatData *cd, size_t chNum, int isTeam);

CombatData **getTeam(CombatData **dest, size_t *teamNum, 
					CombatData *cd, size_t chNum, int isTeam);

CombatData **getOppTeam(CombatData **dest, size_t *teamNum, 
					CombatData *cd, size_t chNum, int isTeam);

void combatPrintCharac(Charac *ch);

void combatPrintTeams(Charac *ally, size_t allyNum,
						Charac *enemy, size_t enemyNum);

int combatAttack(CombatData *a, CombatData *b);

CombatData *combatChooseTarget(CombatData *cd, size_t chNum);

int combatFleeAttempt(CombatData *cd, size_t chNum, size_t ind);

int combatPlayerTurn(CombatData *cd, size_t chNum, size_t ind);

int combatEnemyTurn(CombatData *cd, size_t chNum, size_t ind);

void runCombat(Charac *ally, size_t allyNum,
				Charac *enemy, size_t enemyNum);



#endif