


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




//		INITIALIZATION

// Writes to log file
void log_msg(char *msg, ...){
	va_list args;
    va_start(args, msg);
    vfprintf(LOG, msg, args);
    va_end(args);
}


// Opens log file
Error log_init()
{
	LOG = fopen(LOGFILE, "a");
	if(!LOG){
		fprintf(stderr, " Error: unable to open logging file at %s\n", LOGFILE);
		return FILE_ERROR;
	}

	// Log current time
	time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );

	log_msg("\n\n[NEW GAME INSTANCE] (%d/%d/%d, %d:%d)\n",
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
		log_msg(" Error: failed to allocate memory\n");
		return MEM_ALLOC_ERROR;
	}

quit:
	return err;
}


/* Frees every game object */
void game_free()
{
	//Free vectors
	vfree(ITEMS);
	vfree(PLAYERS);
	vfree(CHARACS);

	log_msg("Closing game...\n");
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
		int read_num = sscanf(line, "%d;%[^;];%d;%d;%d;%d\n", 
						&new.id, new.name,
						&new.type, &new.eqp_type,
						&new.price, &new.data);

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
		
		Charac new;
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

// Read Character Data from TXT
/*
Error charac_read_data(vector *v, const char *path)
{
	
	//Number of expected data columns in data, defined by relevant struct
	size_t EXP_COLS = 6;
	//Array stores number of rows and columns
	size_t shape[2];

	//Get data from txt
	char **raceData = GenFromTxt(path, &shape[0], STR_LINE_MAX, ';', '#');
	if(!raceData){
		log_msg(" Error: could not read %s\n", CHARACS_TXT);
		return FILE_ERROR;
	}

	//Check expected columns
	if(shape[1] != EXP_COLS){
		log_msg(" Error: wrong formatting in %s\n", CHARACS_TXT);
		return FILE_ERROR;
	}

	//Save txt data in item vector
	for(size_t i=0; i<shape[0]; i++)
	{

		//Retrieve in order:
		// Name (string), type (int), CON, STR, DEX, INT (ints)
		Charac temp;

		//Get ID
		if(!strtoint(&temp.id, raceData[i*shape[1]])){
			log_msg(" Error: wrong formatting in %s\n", CHARACS_TXT);
			return FILE_ERROR;
		}

		//Get name
		strcpy(temp.name, strstripw(raceData[i*shape[1]+1]));

		int *data[] = {&temp.type, &temp.cons, &temp.strn, &temp.dext, &temp.intl};
		for(int i=0; i<5; i++)
		{
			if(!strtoint(data[i], raceData[i*shape[1] + i + 2])){
				log_msg(" Error: wrong formatting in %s\n", CHARACS_TXT);
				return FILE_ERROR;
			}
		}

		//Save new item in vector
		vinsert(v, vsize(v), &temp);
	}

	//Free GenFromTxt return array
	for(size_t i=0; i<shape[0]*shape[1]; i++){
		free(raceData[i]);
	}
	free(raceData);
	log_msg("Finished reading character data (%"SZ_FMT" bytes)\n", vmem(CHARACS));
	return NO_ERROR;
}
*/


// Wrap function to read all data
Error game_read_data()
{
	Error err = NO_ERROR;

	// Items
	err = item_read_data(ITEMS, ITEMS_TXT);
	check_quit;

	err = charac_read_data(CHARACS, CHARACS_TXT);
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




//	ITEMS
// Search by name, returns first instance,
// and NULL otherwise
Item *item_search_name(vector *items, const char *lookup_name)
{
	for(size_t i=0; i<vsize(items); i++)
	{
		Item *temp = vat(items, i);
		if (strcmp(temp->name, lookup_name) == 0){
			return temp;
		}
	}
	return NULL;
}

// Search by ID, returns first instance,
// and NULL otherwise
Item *item_search_id(vector *items, int lookup_id)
{
	for(size_t i=0; i<vsize(items); i++)
	{
		Item *temp = vat(items, i);
		if (temp->id == lookup_id){
			return temp;
		}
	}
	return NULL;
}
