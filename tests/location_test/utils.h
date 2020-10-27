#ifndef _UTILS_HEADER_H
#define _UTILS_HEADER_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*


	===== List of Utility Functions =====

	drand - return random integer between 1 and max.
	rolld20 - return random integer between 1 and 20.
	
	strslc - slices string at tho points
	getstr - get input string from user
	getAChar - asks user for a character
	strtoint - convert string into integer
	strtointArr - convert strings into integer array
	strtoflt -  convert string to float
	strtofltArr - convert strings into float array
	
	strtokn - counts tokens in string given delimiter
	strpar - partitions string one token at a time
	strsplit - splits string into tokens

	CountTxtLines - counts lines in file
	ReadTxtLines - reads file lines and stores in string array
	GenFromTxt - reads data from file and returns allocated 
					string array

	--- WIP ---

	SaveToTxt - saves data to txt file
	inttostr - convert integer to string
	inttostrArr - convert integer array to string array
	flttostr - convert double to string
	flttostrArr - convert double array to string array

*/



// 	============== GAME UTILS ==============


typedef unsigned int uint;

/* Allocates memory, terminates the program on fail */
void * xmalloc(size_t bytes);

/* Returns a random number between 1 and the input value */
int drand(int max);
/* Returns a random integer between 1 and 20 */
int rolld20();
/* Returns a random integer between 1 and 6 */
int rolld6();
/* Prints 10 newline characters */
void clear();


// ========= DATA MANIPULATION =========


/* Finds maximum of array */
int
intmax(const int *arr, size_t len);

/* Finds minimum of array */
int
intmin(const int *arr, size_t len);

/* Finds index of maximum of array */
size_t
intimax(const int *arr, size_t len);

/* Finds index of minimum of array */
size_t
intimin(const int *arr, size_t len);

/*
Slices the input string from index i to index j,
keeping both 'i' and 'j'.
Returns string pointer, and NULL on fail.
*/
char *
strslc(char *s, size_t i, size_t j);

/*
Remove spaces from a string
*/
char *
strNoSpaces(char *str);


/*
Asks user for input with a message,
and returns a string.
*/
char *
getstr(char *dest, const char *msg);

/* Asks the user for a character */
char getAChar();

/* Asks user for integer. On fail it returns input value */
int getint(int fail);


/*
Converts a input string 'str' into an integer.
The result is stored at the input pointer 'dest'.
On fail, NULL is returned.
*/
int *
strtoint(int *dest, const char *str);



/*
Converts and array of strings of length 'size' 
into an array of integers.
Values are stored in memory of input array 'dest'.
*/
int *
strtointArr(int *dest, char **str, size_t size);



/*
Convert string to double.
*/
double *
strtoflt(double *dest, const char *str);



/*
Converts and array of strings of length 'size' 
into an array of doubles.
Values are stored in memory of input array 'dest'.
*/
double *
strtofltArr(double *dest, const char **str, size_t size);



/*
Given a string and a delimiter,
it returns the number of tokens in the string.
*/
size_t
strtokn(const char *str, char delim);



/*
Given a string and a delimiter,
it runs through the first field
of the string until the delimiter,
substitutes the delimiter by a "\0",
and returns a pointer to the next field.
If the end of the string is reached, a NULL
pointer is returned.
*/
char *
strpar(char *ptr, const char delim);



/*
Given a string 'str' and a delimiter 'delim',
it splits the string at the delimiters, stores
the pointers to the tokens in 'dest', and returns
a pointer to it. On fail, it returns NULL.
Input:
	char **dest:	pointer to array of string pointers
					in which tokens are saved.
	char *str:		string to split into tokens.
	char delim:		delimiter character
Output:
	char **dest:	pointer to 'dest' defined above.
	NULL:			if unsuccessful
*/
char **
strsplit(char **dest, char *str, const char delim);




// ========= INPUT / OUTPUT =========


/*
Counts the lines in a txt file,
and returns the value.
The counting process discards empty lines
and comment lines, defined by an input starting character.

Input:
	size_t maxRead:	maximum size in which to store a file line.
	char *path:		string to file path.
	char *comment:	pointer to character that defines file comment.
Output:
	size_t lineCount:	number of non-comment non-empty lines in file.
*/
size_t
CountTxtLines(size_t maxRead, const char *path, const char *comment);




/*
Reads the non-comment lines from a txt 
and returns the strings.
One should first call CountTxTLines, then initialize
enough memory.

Input:
	char *path: 	string with file path.
	char **lines: 	pointer to array of pointers of strings
					with enough memory to hold all the lines.
	size_t MaxRead: memory size of each line in chars.
	char *comment: 	comment character.

Output:
	char **lines:	input pointer defined above.
	NULL:			if file not found.
*/
char **
ReadTxtLines(char **lines, size_t maxRead, 
				const char *path, const char *comment);





/*
Reads data from a file, separated into rows and columns
by an input delimiter.

Input:
	size_t *shape: 	pointer to array of two members, stores
					number of rows and columns of data.
	size_t maxSize:	maximum size in which to store file line.
	char *path: 	string with file path.
	char delim:		character data delimiter.
	char comment:	character that indicates a line to skip
					in the file.
Output:
	char **data:	pointer to allocated memory of size
					shape[0] * shape[1] * maxSize
					Individual strings can be accessed via
					data[r*cols + c]
					where 'r' and 'c' are the row and column
					of the string, and 'cols' is the total number
					of columns.
					As it is allocated, it must be freed afterwards.
					Both the pointer **data and the individual strings
					must be freed.
	NULL:			If file not found, empty, or has uenxpected format;
					and if memory allocation error occurs.
*/
char **
GenFromTxt(const char *path, size_t *shape, const size_t maxSize, 
			const char delim, const char comment);



/*
Checks whether a file of input path exists.
Returns path pointer, and NULL on fail.
*/
char *checkFile(const char* path);



// ---------------------------------------------

// 			VECTOR


typedef struct vectorStruct
{
	void *d;
	size_t size;
	size_t dtype;
} vector;



/*
Allocates new vector and returns pointer to it
*/
vector *vnew(size_t bytes);


//		GETTERS
/*
Returns the size of the vector
*/
size_t vsize(vector *v);

/*
Returns the memory size of the
vector members' data type, in bytes
*/
size_t vdtype(vector *v);

/* Returns pointer to data array of vector */
void *vdata(vector *v);

/*
Returns a pointer to a vector member.
Must be cast to the appropriate data type.
e.g.
	double *var = *(double*) vat(v,0)
returns a pointer to the first member (index 0)
of a vector 'v' and casts it to a double.
*/
void *vat(vector *v, size_t i);



//		SETTERS


/* Changes the value of a vector member */
vector *vset(vector *v, size_t i, void *src);

/*
Substitutes every member in the vector with
the input member
*/
vector *vfill(vector *v, void *src);



//		SIZE MANIPULATION

/*
Inserts a new member into the vector
at index 'j'.
*/
vector *vinsert(vector *v, size_t j, void *new);

/* Removes member 'i' from vector */
vector *vdelete(vector *v, size_t i);

vector *vresize(vector *v, size_t newsize);

void vfree(vector *v);




#endif


