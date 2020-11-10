#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../../include/utils.h"


/*

	Locations Test

	Reads a number of location data from a text file,
	and its connections with other locations.

	Locations are stored as an array of structures
	in the same memory zone.
	Each location has an array of 5 pointers to other
	locations. These pointers represent
	locations to which this place is connected to
	via roads.

	A Travelling demo is included.
	It gives the user a random starting location,
	and allows them to change location by "travelling the roads".

	TO-DO:
		drand() always returns 1 on first call.
		Reduce size of loc_read, spit into functions.
	


	LOCATIONS TEST: STAGE II

	- Develop setting for specific locations:
		* Town activities, buildings, etc
		* Dungeon generator
		* ETC
*/


// Removes the whitespaces from both sides of a string
// leaving interior whitespaces untouched.
char *strstrip_lr(char *str)
{
	char buff[strlen(str)+1];
	strcpy(buff, str);

	// Remove left whitespaces
	char *left = buff;
	while( isspace(*left) )
	{
		left++;
	}
	if(*left == '\0'){
		return left;
	}
	// Remove right whitespaces
	char *right = buff + strlen(buff) - 1;
	while(isspace(*right)){
		right--;
	}
	*(right+1) = '\0';

	strcpy(str, buff);
	return str;
}



#define STR_NAME_MAX 30
#define STR_LINE_MAX 100

#define LOC_MAX_ROADS 5


enum loc_type{
	LOC_TOWN,	// Town, city
	LOC_DUN,	// Dungeon
	LOC_CAMP,	// Settlement, camp
	LOC_NONE = -1
};

typedef struct location_struct{
	char name[STR_NAME_MAX];
	int id;
	int type;
	int road_id[LOC_MAX_ROADS];
	struct location_struct *roads[LOC_MAX_ROADS];
} Loc;


/*
Reads location data from text file.
Returns a vector with location structures.
*/
vector *loc_read()
{
	// Variable declarations
	vector *locations = vnew(sizeof(Loc));
	FILE *loc_data;
	char *loc_txt = "loc_data.txt";

	// Stores strings read
	char line[STR_LINE_MAX+1];
	
	// Stores return value of sscanf
	//int ret;
	//int num_read;

	// File reading
	loc_data = fopen(loc_txt, "r");

	// Safe because stores up to STR_LINE_MAX characters in line,
	// which has a size of STR_LINE_MAX + 1 spaces.
	while(fgets(line,STR_LINE_MAX,loc_data) != NULL)
	{
		// Skip comments
		if(line[0] == '#'){
			continue;
		}
		
		Loc new;

		// Read info about location itself
		char data[4][STR_LINE_MAX+1];
		char *data_ptr[4] = {data[0], data[1], data[2], data[3]};
		char rem[STR_LINE_MAX+1];

		if(strtokn(line, ',') != 4){
			goto bail_out;
		}
		strsplit(data_ptr, line, ',');

		if( !strtoint(&new.id, data_ptr[0]) || !strtoint(&new.type, data_ptr[2])){
			goto bail_out;
		}
		
		strcpy(new.name, strstrip_lr(data_ptr[1]) );
		strcpy(rem, data_ptr[3]);
		

		// Stores road IDs
		int ids[LOC_MAX_ROADS] = {-1, -1, -1, -1, -1};

		// Reads IDs of locations to which its connected
		size_t rem_tokens = strtokn(rem, ';');
		if( rem_tokens > LOC_MAX_ROADS ){
			goto bail_out;
		}

		char data_rem[rem_tokens][STR_LINE_MAX+1];
		char *data_ptr_rem[rem_tokens];
		for(size_t i=0; i<rem_tokens; i++){
			data_ptr_rem[i] = data_rem[i];
		}
		strsplit(data_ptr_rem, rem, ';');

		if(!strtointArr(ids, data_ptr_rem, rem_tokens)){
			goto bail_out;
		}

		for(int i=0; i<LOC_MAX_ROADS; i++){
			new.road_id[i] = ids[i];
		}
		vinsert(locations, vsize(locations), &new);
	}
	fclose(loc_data);

	//Retrieve location pointers from list of IDs.
	for(size_t i=0; i<vsize(locations); i++)
	{
		Loc *curr = vat(locations, i);
		for(size_t j=0; j<LOC_MAX_ROADS; j++){
			if(curr->road_id[j] == -1){
				curr->roads[j] = NULL;
				continue;
			}
			curr->roads[j] = vat(locations, curr->road_id[j]);
		}
		//Replace dashes on names with spaces
		
		for(size_t j=0; j<strlen(curr->name); j++){
			if(curr->name[j] == '-'){
				curr->name[j] = ' ';
			}
		}
	}

	return locations;

bail_out:
	fprintf(stderr, " Error: unexpected format in %s\n", loc_txt);
	vfree(locations);
	return NULL;
}



void travelling_demo(vector *locations)
{
	// Why does it return 1 the first time it's called?
	int rand = drand((int)vsize(locations));


	rand = drand((int)vsize(locations));
	int start_id = rand - 1;
	Loc *curr = vat(locations, start_id);


	while(1)
	{
		printf(" You are in %s\n", curr->name);
		printf(" Where would you like to travel to?\n");

		for(size_t i=0; i<LOC_MAX_ROADS; i++)
		{
			if(curr->roads[i] == NULL){
				continue;
			}
			printf(" %d) %s\n", (int)i, curr->roads[i]->name);
		}
		printf(" x) Exit\n");
		
		int opt = (int)(getAChar() - '0');
		if((char)opt + '0' == 'x'){
			break;
		}
		if(opt >= LOC_MAX_ROADS || curr->roads[opt] == NULL){
			printf(" Invalid option\n");
			continue;
		}
		
		curr = curr->roads[opt];
	}
}


int main()
{

	// read location data from file
	vector *locations = loc_read();

	// Printing test
	printf(" %d locations read:\n", (int)vsize(locations));
	for(size_t i=0; i<vsize(locations); i++)
	{
		Loc *curr = vat(locations,i);

		printf(" %d) %s ", curr->id, curr->name);
		printf(" (");
		for(size_t k=0; k<5; k++){
			printf("%d,", curr->road_id[k]);
		}
		printf(")\n");

		for(size_t j=0; j<LOC_MAX_ROADS; j++){
			if(curr->roads[j] != NULL){
				printf("\t->%s\n", curr->roads[j]->name);
			}
		}
	}



	// Travelling demo
	printf("\n\n\n");
	srand(time(NULL));
	travelling_demo(locations);
	

	vfree(locations);
	return 0;
}
