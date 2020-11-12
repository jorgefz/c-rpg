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
Stores temporal combat-specific information about a character.
*/
typedef struct combatDataStruct
{
	// Character affected
	Charac *ch;

	// '1' if yes, '0' if enemy
	int is_ally;

	// Initiative
	int init;

	// Index in intiative sorted array
	size_t ind_sort;
	
	// '1' yes, '0' no
	int is_dead;
	
	// Check to skip turn if stunned, etc
	int is_stun;

	// Checks if player has chosen defend option this turn.
	int is_defending

	// Number of turns until stun wears off.
	int counter_turn_stun;

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