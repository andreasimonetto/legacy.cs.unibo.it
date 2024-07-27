#include "string.h"

void* memset(void *s, int c, size_t size)
{
unsigned i;
	
	for(i = 0; i < size; i++)
		*((char*)s) = c;
	return s;

}

void* memcpy(void *dest, const void *src, size_t size)
{
unsigned i;
	
	for(i = 0; i < size / sizeof(int); i++) {
		((int*)dest)[i] = ((int*)src)[i];
	}
	
	if(size % sizeof(int) != 0) {
		for(i *= 4; i < size; i++) {
			((char*)dest)[i] = ((char*)src)[i];
		}
	}
	return dest;
}

int strcmp(const char *str1, const char *str2)
{
	if(!str1)
		return str2 ? -1 : 0;
	else if(!str2)
		return 1;
	
	while(*str1 && *str2) {
		if(*str1 < *str2)
			return -1;
		else if(*str1 > *str2)
			return 1;
		
		str1++;
		str2++;
	}
	
	if(*str1)
		return -1;
	
	return *str2 ? 1 : 0;
}

char *strcpy(char *dest, const char *src)
{
int i;
	
	if(!dest || !src)
		return NULL;
	
	for(i = 0; (dest[i] = src[i]); i++);
	return dest;
}

int strlen(const char *str)
{
int i;
	
	if(!str)
		return 0;
	
	for(i = 0; str[i]; i++);
	return i;
}
