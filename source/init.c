
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "include/utils.h"
#include "include/init.h"


// 		GLOBAL VARIABLES

// Current number of players in team
int num_players = 1;


// Data reading directories
const char *ITEMS_TXT = "../data/items.txt";
const char *CHARACS_TXT = "../data/characs.txt";
// Save game location
const char *SAVE_DIR = "../data/saves/";
// Location to which log game processes
const char *LOGFILE = "../data/logs/log.txt";

const char *item_attrib_names[][2] = {
	{"Weapon melee damage", "Weapon melee weight"},
	{"Armour defense rating", "Armour weight"},
	{"Ammo attribute 1", "Ammo attribute 2"},
	{"Potion strength", "Potion affects"},
	{"Misc item attrib1", "Misc item attrib2"},
	{"Weapon Ranged damage", "Weapon ranged attrib2"},
};




//		INITIALIZATION

// Writes to log file
void log_msg(char *msg, ...){
	va_list args;
    va_start(args, msg);
    vfprintf(LOG, msg, args);
    fflush(LOG);
    va_end(args);
}


// Opens log file
Error log_init()
{
	LOG = fopen(LOGFILE, "w");
	if(!LOG){
		fprintf(stderr, " Error: unable to open logging file at %s\n", LOGFILE);
		fprintf(stderr, " Please, make sure you execute the game from the bin folder\n");
		return FILE_ERROR;
	}

	// Log current time
	time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );

	log_msg("[NEW GAME INSTANCE] (%d/%d/%d, %d:%d)\n",
						timeinfo->tm_mday,
						timeinfo->tm_mon + 1,
						timeinfo->tm_year + 1900,
						timeinfo->tm_hour,
						timeinfo->tm_min);

	return NO_ERROR;
}






/* Initalises and allocates game objects */
Error game_init()
{
	Error err = NO_ERROR;

	// Begin logging
	err = log_init();
	check_quit;

	// ALLOCATE STRUCTURE VECTORS
	ITEMS = vnew(sizeof(Item));

	PLAYERS = vnew(sizeof(Charac));

	CHARACS = vnew(sizeof(Charac));

	if(!ITEMS || !PLAYERS || !CHARACS){
		log_msg(" Error: failed to allocate memory for game assets\n");
		return MEM_ALLOC_ERROR;
	}

quit:
	return err;
}


/* Frees every game object */
void game_free(int err)
{
	//Free vectors
	vfree(ITEMS);
	vfree(PLAYERS);
	charac_free();

	log_msg("Closing game...\n");
	log_msg("Exited with error code %d\n", err);
	fclose(LOG);
}



//______________________________________________
//		READ DATA FROM TXT
//______________________________________________

Error item_read_data()
{
	Error err = NO_ERROR;
	char line[STR_LINE_MAX];
	int line_counter = 0;
	
	FILE *item_data = fopen(ITEMS_TXT, "r");
	if(!item_data){
		log_msg(" Error: could not open %s\n", ITEMS_TXT);
		return FILE_ERROR;
	}

	while(fgets(line, STR_LINE_MAX-1, item_data) != NULL)
	{
		line_counter++;
		// Skip comments and empty lines
		if(line[0] == '#' || line[0] == '\n'){
			continue;
		}
		
		Item new;
		/*
		%[^;] reads everything until it finds a semicolon,
		avoding the problems sscanf has when reading
		strings with whitespaces.
		*/
		int read_num = sscanf(line, "%d;%[^;];%d;%d;%d;%d;%d\n", 
						&new.id, new.name,
						&new.type, &new.eqp_type,
						&new.price,
						&new.attrib[0], &new.attrib[1]);

		if(read_num != ITEM_DATA_COLS){
			err = FILE_ERROR;
			log_msg(" Error reading %s (line %d)\n", ITEMS_TXT, line_counter);
			goto quit;
		}
		// Remove whitespaces from left and right of string name
		strstripw_lr(new.name);
		// Add to global items vector
		vinsert(ITEMS, vsize(ITEMS), &new);
	}

	fclose(item_data);
	log_msg(" %d elements read from %s (%"SZ_FMT" bytes)\n", vsize(ITEMS), ITEMS_TXT, vmem(ITEMS));
quit:
	return err;
}



Error charac_read_data()
{
	Error err = NO_ERROR;
	char line[STR_LINE_MAX];
	int line_counter = 0;
	
	FILE *charac_data = fopen(CHARACS_TXT, "r");
	if(!charac_data){
		log_msg(" Error: could not open %s\n", CHARACS_TXT);
		return FILE_ERROR;
	}

	while(fgets(line, STR_LINE_MAX-1, charac_data) != NULL)
	{
		line_counter++;
		// Skip comments and empty lines
		if(line[0] == '#' || line[0] == '\n'){
			continue;
		}
		// Create character with default values
		Charac new = {.inv = NULL, .magic = NULL,
					.eqp_slots = {NULL}};
		/*
		%[^;] reads everything until it finds a semicolon,
		avoding the problems sscanf has when reading
		strings with whitespaces.
		*/
		int read_num = sscanf(line, "%d;%[^;];%d;%d;%d;%d;%d\n", 
						&new.id, new.name, &new.type,
						&new.cons, &new.strn, &new.dext, &new.intl);

		if(read_num != CHARAC_DATA_COLS){
			err = FILE_ERROR;
			log_msg(" Error reading %s (line %d)\n", CHARACS_TXT, line_counter);
			goto quit;
		}
		// Remove whitespaces from left and right of string name
		strstripw_lr(new.name);
		// Add to global items vector
		vinsert(CHARACS, vsize(CHARACS), &new);
	}

	fclose(charac_data);
	log_msg(" %d elements read from %s (%"SZ_FMT" bytes)\n", vsize(CHARACS), CHARACS_TXT, vmem(CHARACS));
quit:
	return err;
}


// Wrap function to read all data
Error game_read_data()
{
	Error err = NO_ERROR;

	// Items
	err = item_read_data();
	check_quit;

	err = charac_read_data();
	check_quit;

	//Characs
	//etc
	log_msg("Finished reading game data\n");
quit:
	return err;
}






//______________________________________________
//		STRUCT INDEXING
//______________________________________________


// Characters

/*
Retrieve characters of given type
in a vector of pointers.
Note that in return vector, to access charac data, use:
	Charac *race_i = *(Charac**)vat(race_list, i)
*/
vector *charac_search_type(int intype)
{
	vector *type_list = vnew(sizeof(Charac*));
	for(size_t i=0; i<vsize(CHARACS); i++)
	{
		Charac *cur = vat(CHARACS, i);
		if(cur->type == intype){
			vinsert(type_list, 0, &cur); 
		}
	}
	return type_list;
}


/*
Retrieve races from character list.
*/
vector *charac_get_races()
{
	vector *race_list = charac_search_type(CHARAC_RACE);
	return race_list;
}


/*
Retrieves a character that has the input name.
*/
Charac *charac_search_name(const char *name)
{
	for(size_t i=0; i<vsize(CHARACS); i++)
	{
		Charac *cur = vat(CHARACS, i);
		if( strcmp(cur->name, name) == 0 ){
			return cur;
		}
	}
	return NULL;
}

/*
Retrieves a character of input id.
*/
Charac *charac_search_id(int id)
{
	for(size_t i=0; i<vsize(CHARACS); i++)
	{
		Charac *cur = vat(CHARACS, i);
		if( cur->id == id ){
			return cur;
		}
	}
	return NULL;
}

/*
Frees the memory of all characters data
*/
void charac_free()
{
	// Freeing character inventories
	for(size_t i=0; i<vsize(CHARACS); i++)
	{
		Charac *cur = vat(CHARACS, i);
		if(cur && cur->inv){
			vfree(cur->inv);
		}
	}
	vfree(CHARACS);
}



//	ITEMS

/*
Retrieves a vector list of items of input type.
*/
vector *item_search_type(int intype)
{
	vector *type_list = vnew(sizeof(Item*));
	for(size_t i=0; i<vsize(ITEMS); i++)
	{
		Item *cur = vat(ITEMS, i);
		if(cur->type == intype){
			vinsert(type_list, 0, &cur); 
		}
	}
	return type_list;
}



/*
Returns the first item with the input name.
*/
Item *item_search_name(const char *lookup_name)
{
	for(size_t i=0; i<vsize(ITEMS); i++)
	{
		Item *temp = vat(ITEMS, i);
		if (strcmp(temp->name, lookup_name) == 0){
			return temp;
		}
	}
	return NULL;
}


/*
Returns an item with the input id.
*/
Item *item_search_id(int lookup_id)
{
	for(size_t i=0; i<vsize(ITEMS); i++)
	{
		Item *temp = vat(ITEMS, i);
		if (temp->id == lookup_id){
			return temp;
		}
	}
	return NULL;
}


/*
Retuns the string name fro an item attribute
*/
const char *item_get_attrib_name(int type, int attrib)
{
	return item_attrib_names[type][attrib];
}