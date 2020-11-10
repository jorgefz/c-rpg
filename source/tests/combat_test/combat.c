#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utils.h"
#include "setup.h"
#include "combat.h"


/*
void combatPrintAttackMsg()

	Charac *attacker
	Charac *victim

	Item *weaponUsed
	Magic *spellUsed

	int damage
	bool isCritical
	bool missed
	bool usedShield
{
	Normal attack:
	Unarmed:"P1 punched P2, causing D damage!"
	Sword:"P1 slashed P2 with their sword, causing D damage!"
	Melee:"P1 smashed P2 with their hammer, causing D damage!"
	Bow:"P1 shot an arrow at P2, causing D damage!"
	Magic:"P1 burnt P2 with their S spell, causing D damage!"
	Magic:"P1 froze P2 with their S spell, causing D damage!"

	Failing (non shield):
	Unarmed:"P2 dodged P1's kick!"
	Sword:"P1 slashed their sword at P2, but missed!"
	Melee:"P1 swept their hammer at P2, but missed!"
	Bow:"P1's arrow missed P2!"
	Magic:"P1's S spell missed P2!"

	Failing (shield):
	Unarmed:"P1 kicked P2's shield, causing no damage!"
	Sword:"P1 slashed their sword at P2, but P2 blocked it!"
	Melee:"P1 swept their hammer at P2, but P2 blocked it!"
	Bow:"P1's arrow bounced off P2's shield!"
	Magic:"P2 deflected P1's spell!"

	Critical:
	Unarmed:"Critical strike! P1 striked P2's head"
	Sword:"Critical strike! P1's sword thrusted through P2, causing D damage!"
	Melee:"Critical strike!" P1's hammer crushed P2's chest, causing D damage!"
	Bow:"Critical strike! P1 set P2 ablaze with their S spell, causing D damage!"
	Magic:"Critical strike! P1 froze P2 with their S spell, causing D damage!"

	Killing (either normal or critical):
	"P1 chopped P2's head off!"
	"P1 crushed P2's head into sludge!"
	"P1's arrow pieced P2 right through the heart!"
	"P1 incinerated P2!"
	"P1 froze P2 into an ice block, which the broke into a hundred pieces!"


}
*/


/*
Gets number of players on a team specified by input 'team',
where it can take the values 1 (allies) or 0 (enemies).
*/
size_t getTeamNum(CombatData *cd, size_t chNum, int isTeam)
{
	size_t teamNum = 0;
	for(size_t i=0; i<chNum; i++){
		if( cd[i].isAlly == isTeam )
			teamNum++;
	}
	return teamNum;
}

/*
Retrieves the members of a specified team
	**dest		Array of pointers where team members are stored.
				It must have space for all characters in battle.
	*teamNum 	Pointer that will store number of characters in team.
	*cd 		CombatData array of all characters in battle.
	chNum		number of characters in battle.
	isAlly		team ID you seek
*/
CombatData **getTeam(CombatData **dest, size_t *teamNum, 
					CombatData *cd, size_t chNum, int isTeam)
{
	size_t counter = 0;
	for(size_t i=0; i<chNum; i++){
		if( cd[i].isAlly == isTeam ){
			dest[counter] = &cd[i];
			counter++;
		}
	}
	*teamNum = counter;
	return dest;
}

/* Exactly the same as getTeam(), but returns the opposite team */
CombatData **getOppTeam(CombatData **dest, size_t *teamNum, 
					CombatData *cd, size_t chNum, int isTeam)
{
	//Switch team value
	if (isTeam == 1)
		isTeam = 0;
	else
		isTeam = 1;
	//Loop over characters
	size_t counter = 0;
	for(size_t i=0; i<chNum; i++){
		if( cd[i].isAlly == isTeam ){
			dest[counter] = &cd[i];
			counter++;
		}
	}
	*teamNum = counter;
	return dest;
}



/* Prints an input character's main stats */
void combatPrintCharac(Charac *ch)
{
	printf(" -------------------------------- \n");
	printf(" %s)", ch->name);
	printf(" HP:%d/%d", ch->HP, ch->maxHP);
	printf(" SP:%d/%d", ch->SP, ch->maxSP);
	printf(" MP:%d/%d\n", ch->MP, ch->maxMP);
	return;
}


/* Prints main stats of characters in both teams */
void combatPrintTeams(Charac *ally, size_t allyNum,
						Charac *enemy, size_t enemyNum)
{
	clear();
	for(size_t i=0; i<allyNum; i++)
		combatPrintCharac(&ally[i]);
	printf(" -------------------------------- \n");
	printf("             VS \n");
	for(size_t i=0; i<enemyNum; i++)
		combatPrintCharac(&enemy[i]);
	printf(" -------------------------------- \n");

	return;
}


/*
Given an attacking character and a victim one,
calculates the damage caused.
*/
int combatAttack(CombatData *a, CombatData *b)
{
	Charac *attacker = a->ch;
	Charac *victim = b->ch;

	//What if player has no main weapon?
	int damage = 0;
	int abMod = 0;
	int roll = drand(attacker->mainHand->data);
	
	//Melee damage
	if(attacker->mainHand->type == 'm')
		abMod = getAbMod(attacker->strn);
	//Ranged damage
	else if (attacker->mainHand->type == 'r')
		abMod = getAbMod(attacker->strn);

	//NOTE: Add proficiency bonus when Skills are added
	damage = roll + abMod;

	//Roll exceptions
	if(roll == 20){
		damage = damage*2;
		printf(" Critical Strike!\n");
		
	}
	else if(roll == 1){
		damage = 0;
		printf(" %s missed!\n", attacker->name);
	}

	// Compare with armor class
	victim->HP -= damage;
	// Avoids negative health
	if(victim->HP < 0)
		victim->HP = 0;

	printf(" %s deals %d damage\n", attacker->name, damage);
	getAChar();

	return damage;
}


/*
	Prints non-dead enemies, and lets player choose which one
	to attack. The target index is returned.
*/
CombatData *combatChooseTarget(CombatData *cd, size_t chNum)
{
	int retInd;
	while(1){
		//List targets
		printf(" Choose your target:\n");
		size_t attackInd[chNum];
		size_t count = 0;
		for(size_t i=0; i<chNum; i++){
			if (cd[i].isAlly == 1)
				continue;
			if (cd[i].ch->HP <= 0)
				continue;
			printf(" %Iu) %s", count+1, cd[i].ch->name);
			printf(" (%d/%d)\n", cd[i].ch->HP, cd[i].ch->maxHP);
			attackInd[count] = i;
			count++;
		}
		printf(" 0) Back\n");

		//Choose
		int option = getint(-1) - 1;
		//Go back check
		if (option == -1){
			return NULL;
		}
		if(option < 0 || option >= (int)count)
			continue;
		retInd = attackInd[option];
		break;
	}
	return &cd[retInd];
}


/*
	Attempts to flee player's team from battle.
	Returns 1 if successful, and 0 otherwise.
*/
int combatFleeAttempt(CombatData *cd, size_t chNum, size_t ind)
{

	// Get members of fleer's team
	size_t fleeNum;
	CombatData *fleeTeam[chNum];
	CombatData **f = getTeam(&fleeTeam[0], &fleeNum, cd, chNum, cd[ind].isAlly);

	size_t chaseNum;
	CombatData *chaseTeam[chNum];
	CombatData **c = getOppTeam(&chaseTeam[0], &chaseNum, cd, chNum, cd[ind].isAlly);

	//Calculate fleeing chances of all team members, and get minimum 
	int fleeVal[fleeNum];
	for(size_t i=0; i<fleeNum; i++)
		fleeVal[i] = rolld6() + getAbMod(f[i]->ch->dext);
	int fleeMin = intmin(&fleeVal[0], fleeNum);

	//Calculate fleeing chances of all enemies, and get maximum
	int chaseVal[chaseNum];
	for(size_t i=0; i<chaseNum; i++)
		chaseVal[i] = rolld6() + getAbMod(c[i]->ch->dext);
	int chaseMax = intmax(&chaseVal[0], chaseNum);

	if(fleeMin >= chaseMax){
		if (cd[ind].isAlly == 1)
			printf(" Your team flees!\n");
		else
			printf(" The enemy team ran away! \n");
		getAChar();
		return 1;
	}
	else{
		if (cd[ind].isAlly == 1)
			printf(" You tried to run away, but the enemy caught up with you!\n");
		else
			printf(" The enemy tried to run away, but you chased them down!\n");
		getAChar();
		return 0;
	}

	return 0;
}


/*
	Print possible choices player has on their turn.
	And execute chosen one.
*/
int combatPlayerTurn(CombatData *cd, size_t chNum, size_t ind)
{
	int flee = 0;
	while(1){
		//Print Options
		printf(" 1) Attack\n");
		printf(" 0) Flee\n");
		int p = getint(-1);

		//Flee
		if(p==0){
			flee = combatFleeAttempt(cd, chNum, ind);
			break;
		}

		//Attack
		else if(p==1){
			CombatData *target;
			target = combatChooseTarget(cd, chNum);
			if (!target)
				continue;
			combatAttack(&cd[ind], target);
			break;
		}
		else
			continue;
	}

	return flee;
}


/*
	AI of enemy turn.
	At the moment, it attacks a random alive player.
*/
int combatEnemyTurn(CombatData *cd, size_t chNum, size_t ind)
{
	size_t allyNum;
	CombatData *allies[chNum];
	getOppTeam(&allies[0], &allyNum, cd, chNum, cd[ind].isAlly);

	//Count non-dead targets
	size_t alive = 0;
	size_t aliveInd[allyNum];
	for(size_t i=0; i<allyNum; i++){
		if(allies[i]->ch->HP > 0){
			aliveInd[alive] = i;
			alive++;
		}
	}

	//Choose randomly
	size_t target = (size_t) drand((int)alive) - 1;
	size_t targetInd = aliveInd[target];
	printf(" %s attacks %s\n", cd[ind].ch->name, allies[targetInd]->ch->name);
	combatAttack(&cd[ind], allies[targetInd]);
	return 0;
}




CombatData *combatSetup(CombatData *cd, Charac *ally, size_t allyNum,
										Charac *enemy, size_t enemyNum)
{
	size_t chNum = allyNum + enemyNum;

	//Calculate initiatives

	// Temporary pre-sorting CombatData struct array
	CombatData temp[chNum];

	// Populating temporary CombatData
	for(size_t i=0; i<allyNum; i++){
		temp[i].ch = &ally[i];
		temp[i].isAlly = 1;
		temp[i].init = rolld20() + getAbMod(ally[i].dext);
		temp[i].indSort = i;
		temp[i].isDead = 0;
		temp[i].isStun = 0;
		temp[i].counterTurnStun = 0;
	}

	for(size_t i=allyNum; i<chNum; i++){
		temp[i].ch = &enemy[i-allyNum];
		temp[i].isAlly = 0;
		temp[i].init = rolld20() + getAbMod(enemy[i-allyNum].dext);
		temp[i].indSort = i;
		temp[i].isDead = 0;
		temp[i].isStun = 0;
		temp[i].counterTurnStun = 0;
	}

	//Sort indices
	 	// CREATE FUNCTIONS IN UTILS

	// Populate final CombatData
	for(size_t i=0; i<chNum; i++)
		cd[i] = temp[i];

	return cd;
}






/*
	Main combat loop.
	Runs through each character in battle
*/
void runCombat(Charac *ally, size_t allyNum,
				Charac *enemy, size_t enemyNum)
{

	size_t chNum = allyNum + enemyNum;
	
	CombatData cd[chNum];
	combatSetup(&cd[0], ally, allyNum, enemy, enemyNum);

	//Combat end condition
	char endCondition = 'c';
	// 'c'ontinue, 'w'in, 'd'efeat, or 'n'eutral (someone flees)
	while(endCondition == 'c')
	{	
		//Loop over all characters in battle
		for(size_t i=0; i<chNum; i++)
		{
			//Skip if dead
			if (cd[i].ch->HP <= 0)
				continue;

			//Print team info
			combatPrintTeams(ally, allyNum, enemy, enemyNum);

			//Print who's turn is it
			printf(" %s's turn...\n", cd[i].ch->name);
			
			//Ally turn
			if (cd[i].isAlly == 1){
				int result = combatPlayerTurn(&cd[0], chNum, i);
				if(result == 1){
					endCondition = 'n';
					break;
				}
			}
			//Enemy turn
			else{
				int result = combatEnemyTurn(&cd[0], chNum, i);
				if(result == 1){
					endCondition = 'n';
					break;
				}
			}
			
			//Count dead in opposite team
			int dead = 0;
			for(size_t j=0; j<chNum; j++){
				if(cd[j].isAlly != cd[i].isAlly && cd[j].ch->HP <= 0)
					dead++;
			}

			// If everyone in a team is dead, then end the battle
			if (cd[i].isAlly == 1 && dead == (int)enemyNum){
				endCondition = 'w';
				break;
			}
			else if (cd[i].isAlly  == 0 && dead == (int)allyNum){
				endCondition = 'd';
				break;
			}
		}
		
	}

	//Based on endCondition, do things
	printf(" End condition was: %c\n", endCondition);
	getAChar();

	//Restore values after battle
	for(size_t i=0; i<allyNum; i++)
		ally[i].HP = ally[i].maxHP;
	for(size_t i=0; i<enemyNum; i++)
		enemy[i].HP = enemy[i].maxHP;

	return;
}