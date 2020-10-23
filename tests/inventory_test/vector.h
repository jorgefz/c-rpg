

#ifndef VECTOR_H
#define VECTOR_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

/*
Returns the size of the vector
*/
size_t vsize(vector *v);

/*
Returns the memory size of the
vector members' data type, in bytes
*/
size_t vdtype(vector *v);

/*
Returns a pointer to a vector member.
Must be cast to the appropriate data type.
e.g.
	double *var = *(double*) vat(v,0)
returns a pointer to the first member (index 0)
of a vector 'v' and casts it to a double.
*/
void *vat(vector *v, size_t i);

/*
Inserts a new member into the vector
at index 'j'.
*/
vector *vinsert(vector *v, size_t j, void *new);

/* Removes member 'i' from vector */
vector *vdelete(vector *v, size_t i);

vector *vresize(vector *v, size_t newsize);

vector *vfill(vector *v, void *src);

void vfree(vector *v);



#endif