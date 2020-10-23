/*
	======= vector.h =======

	This library provides a new object 'vector'
	that behaves as an easily resizeable array
	for any data type and structure.

	To create a new vector, use:
		vector *v = vnew( sizeof(T) );
	where T is the data type of its members.

	To insert a new member 'var' at index 0, use:
		vinsert( v, 0, &var );

	To retrieve the current vector size, use:
		vsize(v);

	To delete a vector member at index 'i', use:
		vdelete( v, i);

	To retrieve a pointer to a specific member
	at index 'i', use:
		T *ptr = vat( v, i );

	To free a vector, use:
		vfree(v);

	To resize a vector to a given size, use:
		vresize(v, size);

	To fill every vector member with the same item, use:
		vfill(v, &item);




	----- Version History -----

	1.0 - 19/07/2020
		- Added basic vector functions:
		vnew, vfree, vsize, vat,
		vinsert, vdelete.
		- Added functions to aid vector manipulation
		vdtype, vresize, vfill



*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"


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

	//Shift memory back over deleted member
	for(size_t j=i; j<vsize(v)-1; j++){
		void *src = vat(v, j+1);
		void *dest = vat(v, j);
		memcpy(dest, src, v->dtype);
	}

	//If new size is 0, free instead
	if(v->size-1 == 0){
		free(v->d);
		v->size--;
		return v;
	}
	
	//Reallocate to reduce memory usage	
	v->d = realloc(v->d, sizeof(v->dtype)*(v->size-1) );	
	v->size--;
	if(v->d == NULL)
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
Substitutes every member in the vector with
the input member
*/
vector *vfill(vector *v, void *src)
{
	for(size_t i=0; i<vsize(v); i++){
		void *dest = vat(v, i);
		memcpy(dest, src, v->dtype);
	}
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
