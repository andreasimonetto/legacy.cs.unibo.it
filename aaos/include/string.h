/** \file string.h
	Strings utility module.
*/
#ifndef _STRING_H
#define _STRING_H
#include "arch.h"

/** \defgroup string Strings
	string.h: Strings and memory manipulation routines.
\{ */

/** Set an area of memory with a value.
	\param s Pointer to the area to set.
	\param c Value you want to set the area.
	\param size The size of memory to set. 
*/
void* memset(void *s, int c, size_t size);

/** Copy an area of memory from src to dest. Memory areas should not overlap.
	\param dest Pointer to the destination area.
	\param src Pointer to the source area.
	\param size The size of memory to copy. 
*/
void* memcpy(void *dest, const void *src, size_t size);

/** Copy a string into a buffer.
	\param dest Destination buffer.
	\param src Source string.
	\return dest or NULL if src is NULL.
*/
char* strcpy(char *dest, const char *src);

/** Calculate the length of a string.
	\param str The string.
	\return The length of str.
*/
int strlen(const char *str);

/** Compares two strings.
	\param str1 First string.
	\param str2 Second string.
	\return -1 if s1<s2, 0 if s1==s2, 1 if s1>s2.
*/
int strcmp(const char *str1, const char *str2);
/* \} */

#endif
