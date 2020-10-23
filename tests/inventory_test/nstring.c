#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct stringStruct
{
	char *s;
	size_t size;
} string;




string *newString(const char *newstr)
{
	string *str = malloc(sizeof(string));

	if(!newstr){
		str->s = NULL;
		size = 0
		return str;
	}

	size_t newsize = strlen(newstr);
	str->s = malloc(sizeof(char)*newsize + 1);
	if(!str->s){
		return NULL;
	}

	memcpy(str->s, newstr, newsize+1);

	return str;
}


string *setString(string *str, const char *newstr)
{
	size_t newsize;
}

string *insertString(string *str, const char *instr, size_t i)
{

}

string *insertChar(string *str, char ch, size_t i)
{

}

/* From i to j */
string *deleteString(string *str, size_t i, size_t j)
{

}

string *deteleChar(string *str, size_t i)
{

}


string *freeString(string *str)
{
	free(str->s);
	free(str);
}