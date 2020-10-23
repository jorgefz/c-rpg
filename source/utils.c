#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#include "utils.h"

/*
	===== Utility Function Definitions =====

*/

void * xmalloc(size_t bytes)
{
	void *mem = malloc(bytes);
	if(!mem){
		fprintf(stderr, "Memory Allocation Error\n");
		exit(-1);
	}
	return mem;
}


int drand(int max)
{
	int r = rand() % max;
	return (r+1);
}

int rolld20()
{
	return drand(20);
}

int rolld6()
{
	return drand(6);
}

void clear()
{
	for(int i=0; i<10; i++)
		printf("\n");
}

//======================================================


int
intmax(const int *arr, size_t len)
{
	/* Initialise max value on first array member */
	int max = arr[0];

	/* Looping through array */
	for(size_t i=0; i<len; i++){
		if( arr[i] > max)
			max = arr[i];
	}
	return max;
}


int
intmin(const int *arr, size_t len)
{
	/* Initialise min value on first array member */
	int min = arr[0];

	/* Looping through array */
	for(size_t i=0; i<len; i++){
		if( arr[i] < min)
			min = arr[i];
	}
	return min;
}


size_t
intimax(const int *arr, size_t len)
{
	/* Initialise max value and its index */
	int max = arr[0];
	size_t imax;

	/* Looping through array */
	for(size_t i=0; i<len; i++){
		if( arr[i] > max ){
			max = arr[i];
			imax = i;
		}
	}
	return imax;
}

size_t
intimin(const int *arr, size_t len)
{
	/* Initialise min value and its index */
	int min = arr[0];
	size_t imin = 0;

	/* Looping through array */
	for(size_t i=0; i<len; i++){
		if( arr[i] < min ){
			min = arr[i];
			imin = i;
		}
	}
	return imin;
}


char *
strslc(char *s, size_t i, size_t j)
{
	/* Input checks */
	if( (i>j) || strlen(s)<(j-i+1) )
		return (NULL);

	/* Move pointer to left slice */
	char *ptr;
	ptr = s + i;

	/* Null terminator after right slice */
	ptr[j-i+1] = '\0';
	
	s = ptr;
	return s;
}


/*
Removes the spaces from a string
*/
char *
strNoSpaces(char *str)
{
	char temp[strlen(str)];
	int c = 0;
	for(size_t i=0; i<strlen(str); i++)
	{
		if(str[i] == ' ')
			continue;
		temp[c] = str[i];
		c++;
	}
	temp[c] = '\0';
	strcpy(str, temp);
	return str;
}



char *
getstr(char *dest, const char *msg)
{
	size_t max_read = (size_t) 1E5;
	printf("%s", msg);
	char *ptr = fgets(dest, max_read, stdin);
	if(!ptr)
		return (NULL);
	// Removes newline character from end of string
	if (strchr(dest, '\n'))
		strslc(dest, 0, strlen(dest)-2);
	return dest;
}


char
getAChar()
{
	char str[100];
	getstr(&(str[0]), " ");
	char ret = str[0];
	return ret;
}


int
getint(int fail)
{
	char str[100];
	char *ret = getstr(&(str[0]), "> ");
	if(!ret)
		return fail;
	int p = str[0] - '0';
	return p;
}


int *
strtoint(int *dest, const char *str)
{
	char *endptr;
	int val;
	if(!str)
		return (NULL);
	//Convert string to integer.
	val = (int) strtol(str, &endptr, 10);
	//Checks if conversion worked
	if(endptr == str)
		return (NULL);
	*dest = val;
	return dest;
}

int *
strtointArr(int *dest, const char **str, size_t size)
{
	for(size_t i=0; i<size; i++){
		int *ret = strtoint(&(dest[i]), str[i]);
		if(!ret)
			return (NULL);
	}
	return dest;
}


double *
strtoflt(double *dest, const char *str)
{
	char *endptr;
	int val;
	if(!str){
		fprintf(stderr, " Error: can't convert NULL to double.\n");
		return (NULL);
	}
	// Converts string to double
	val = (double) strtod(str, &endptr);
	// Checks if conversion worked
	if(endptr == str){
		fprintf(stderr, " Error: can't convert '%s' to double.\n", str);
		return (NULL);
	}
	*dest = val;
	return dest;
}


double *
strtofltArr(double *dest, const char **str, size_t size)
{
	for(size_t i=0; i<size; i++){
		double *ret = strtoflt(&(dest[i]), str[i]);
		if(!ret)
			return (NULL);
	}
	return dest;
}



size_t
strtokn(char *str, const char *delim)
{
	if(!str)
		return 0;
	size_t n = 0;
	for(size_t i=0; i<strlen(str); i++){
		if(str[i] == *delim)
			n++;
	}
	return (n+1);
}



char *
strpar(char *ptr, const char delim)
{
	if(!ptr)
		return (NULL);
 
	while(1)
	{
		if(ptr[0] == delim){
			ptr[0] = '\0';
			ptr++;
			break;
		}

		else if(ptr[0] == '\0')
			return (NULL);
		ptr++;
	}

	return ptr;
}



char **
strsplit(char **dest, char *str, const char delim)
{
	if(!str)
		return (NULL);

	char *ptr = str;
	size_t c = 0;

	while( ptr != NULL ){
		dest[c] = ptr;
		c++;
		ptr = strpar(ptr, delim);
	}

	return dest;
}


//======================================================



size_t
CountTxtLines(size_t maxRead, const char *path, const char *comment)
{
	FILE *fdata;
    fdata = fopen(path, "r");
    if(!fdata){
    	fprintf(stderr, " Error: '%s' file not found.\n", path);
    	return 0;
    }

	char line[maxRead];
    size_t lineCount = 0;
    while(fgets(line, maxRead, fdata))
    {
    	//Ignore comment lines
    	if (line[0] == *comment)
    		continue;
    	//Ignore empty lines
    	if (line[0] == '\n')
    		continue;
    	lineCount++;
    }

    fclose(fdata);
    return lineCount;
}


char **
ReadTxtLines(char **lines, size_t maxRead, 
				const char *path, const char *comment)
{
	// Open file
	FILE *fdata;
    fdata = fopen(path, "r");
    if(!fdata){
    	fprintf(stderr, " Error: '%s' file not found.\n", path);
    	return (NULL);
    }

	char line[maxRead];
    size_t lineCount = 0;
    while(fgets(line, maxRead, fdata))
    {
    	//Ignore comment lines
    	if (line[0] == *comment)
    		continue;
    	//Ignore empty lines
    	if (line[0] == '\n')
    		continue;
    	strcpy(lines[lineCount], line);
    	lineCount++;
    }
    return lines;
}


char **
GenFromTxt(const char *path, size_t *shape, const size_t maxSize, 
			const char delim, const char comment)
{
	//Initialise shape to 0
	shape[0] = 0;
	shape[1] = 0;

	//Counting rows
	size_t lineCount = CountTxtLines(maxSize, path, &comment);
	if(lineCount == 0)
		return (NULL);

	//Initializing pointer to array of lines
	char lines[lineCount][maxSize];
	char *lptr[lineCount];
	for(size_t i=0; i<lineCount; i++)
		lptr[i] = &(lines[i][0]);
	char **l = &(lptr[0]);

	//Reading lines
	ReadTxtLines(l, maxSize, path, &comment);

	//Removing final newline characters
	for(size_t i=0; i<lineCount; i++){
		if(l[i][strlen(l[i])-2] == '\n')
			strslc(l[i], 0, strlen(l[i])-2);
	}

	//Read delimiters from each line to confirm
	//expected number of tokens
	size_t tokens[lineCount];
	for(size_t i=0; i<lineCount; i++){
		tokens[i] = strtokn(l[i], &delim);
	}

	//Compare all tokens to first one
	size_t initToken = tokens[0];
	for(size_t i=0; i<lineCount; i++){
		if (tokens[i] != initToken){
			fprintf(stderr, " Error: wrong number of columns at line %d\n", (int) i);
			return (NULL);
		}
	}

	//Allocate array of strings
	size_t bytes = lineCount*initToken*sizeof(char*);
	char **m = malloc(bytes);
	if(!m){
		fprintf(stderr, " Error: failed to allocate %Iu bytes\n", bytes);
		return (NULL);
	}

	//Allocate individual strings
	int allocFail = (-1);
	for(size_t i=0; i<(lineCount*initToken); i++){
		m[i] = malloc(maxSize*sizeof(char));
		if(!m){
			allocFail = (int)i;
			fprintf(stderr, " Error: failed to allocate %Iu bytes\n", maxSize*sizeof(char));
			break;
		}
	}

	//If failed to allocate a string, free those already allocated
	if(allocFail >= 0){
		for(int i=0; i<allocFail; i++)
			free(m[i]);
		free(m);
		return(NULL);
	}

	//Tokenize
	for(size_t i=0; i<(lineCount); i++){
		//Split lines
		char *ptr = l[i];
		size_t c = 0;
		char *p[initToken];
		while( ptr != NULL ){
			p[c] = ptr;
			c++;
			ptr = strpar(ptr, delim);
		}
		//Save in allocated memory
		for(size_t j=0; j<(initToken); j++)
			strcpy(m[j+i*initToken],p[j]);
	}
	
	//Save shape in input pointer
	shape[0] = lineCount;
	shape[1] = initToken;

	return (m);
}


char *checkFile(const char* path)
{
	FILE *fdata;
    fdata = fopen(path, "r");
    if(!fdata)
    	return (NULL);
    return (char *)path;
}



// --------------------------------------------------

//			VECTOR

/*
Allocates new vector and returns pointer to it
*/
vector *vnew(size_t bytes)
{
	vector *v = malloc(sizeof(vector));
	if(!v)
		return NULL;
	v->d = NULL;
	v->size = 0;
	v->dtype = bytes;
	return v;
}


//		GETTERS

/*
Returns the size of the vector
*/
size_t vsize(vector *v)
{
	return v->size;
}


/*
Returns the memory size of the
vector members' data type, in bytes
*/
size_t vdtype(vector *v)
{
	return v->dtype;
}

/* Returns pointer to data array of vector */
void *vdata(vector *v)
{
	return v->d;
}


/*
Returns a pointer to a vector member.
Must be cast to the appropriate data type.
e.g.
	double *var = *(double*) vat(v,0)
returns a pointer to the first member (index 0)
of a vector 'v' and casts it to a double.
*/
void *vat(vector *v, size_t i)
{
	if(i >= vsize(v))
		return NULL;
	void *ptr = v->d + i*v->dtype;
	return ptr;
}


//		SETTERS

/*
Changes the value of a vector member
*/
vector *vset(vector *v, size_t i, void *src)
{
	if(i >= vsize(v))
		return NULL;
	void *dest = vat(v, i);
	memcpy(dest, src, v->dtype);
	return v;
}

/*
Substitutes every member in the vector with
the input member
*/
vector *vfill(vector *v, void *src)
{
	for(size_t i=0; i<vsize(v); i++)
		vset(v, i, src);
	return v;
}



//		SIZE MANIPULATION

/*
Inserts a new member into the vector
at index 'j'.
*/
vector *vinsert(vector *v, size_t j, void *new)
{
	if(j > vsize(v))
		return NULL;

	//Reallocate with one extra space
	v->d = realloc(v->d, v->dtype*(v->size+1) );
	if(v->d == NULL)
		return NULL;
	v->size++;

	//Shift values forward from insert index
	for(size_t i=vsize(v)-1; i>j; i--)
	{
		void *dest = vat(v, i);
		void *src = vat(v, i-1);
		memcpy(dest, src, v->dtype);
	}

	//Copy new member data from input pointer
	void *dest = vat(v, j);
	memcpy(dest, new, v->dtype);

	return v;
}

/*
Deletes member 'i' from vector
*/
vector *vdelete(vector *v, size_t i)
{
	if(i >= vsize(v))
		return NULL;

	//If there is only one member to delete, free instead
	if(v->size == 1){
		free(v->d);
		v->size--;
		return v;
	}

	//Shift memory back over deleted member
	for(size_t j=i; j<vsize(v)-1; j++){
		void *src = vat(v, j+1);
		void *dest = vat(v, j);
		memcpy(dest, src, v->dtype);
	}

	//Reallocate to reduce memory usage	
	v->d = realloc(v->d, v->dtype*(v->size-1) );	
	v->size--;
	if(!v->d)
		return NULL;

	return v;
}

/*
Changes the size of a vector directly.
Increasing the size will generate new empty members,
and reducing it will erase extra members.
*/
vector *vresize(vector *v, size_t newsize)
{
	v->d = realloc(v->d, sizeof(v->dtype)*newsize);
	if(v->d == NULL)
		return NULL;
	v->size = newsize;
	return v;
}

/*
Frees the vector
*/
void vfree(vector *v)
{
	free(v->d);
	free(v);
}