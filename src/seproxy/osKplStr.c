/** 
Summary

*/


/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#include "osKplStr.h"


/* -------------------------------------------------------------------------- * 
 * - Internal Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- * 
 * - External Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

uint32_t OsKpl_Strlen(const int8_t *s)
{
	return (uint32_t) strlen(s);
}


int8_t* OsKpl_Strcpy(int8_t *s1, const int8_t *s2)
{
	return strcpy(s1, s2);
}


int8_t* OsKpl_Strdup(const int8_t *s)
{
	int8_t *newStr = NULL;
	
	newStr = (int8_t *)_strdup(s);

	return newStr;
}


int8_t* OsKpl_Strncpy(int8_t *s1, const int8_t *s2, uint32_t n)
{
	return strncpy(s1, s2, ((size_t)n));
}


int32_t OsKpl_Strcmp(const int8_t *s1, const int8_t *s2)
{
	return ((int32_t) strcmp(s1, s2));
}


int32_t OsKpl_Strncmp(const int8_t *s1, const int8_t *s2, uint32_t n)
{
	return ((int32_t) strncmp(s1, s2, n));
}


int8_t* OsKpl_Strchr(const int8_t *s, int16_t c)
{
	return strchr(s, c);
}


int8_t* OsKpl_Strrchr(const int8_t *s, int16_t c)
{
	return strrchr(s, c);
}


int32_t OsKpl_StrReverse(int8_t *inOriginal, int8_t *outReversed)
{

	int8_t *p1;
	int8_t *p2;

	if (sizeof(outReversed) < sizeof(inOriginal)) {
		return -1;
	}

	// make p1 point to end of original string
	p1 = inOriginal + OsKpl_Strlen(inOriginal) - 1;

	p2 = outReversed;

	while (p1 >= inOriginal) {
		*p2++ = *p1--;
	}

	// null terminate outReversed
	*p2 = '\0';

	return 0;
}


uint32_t OsKpl_Strtoul(const int8_t *str, int8_t **endptr, int16_t radix)
{
	return ((uint32_t) strtoul(str, endptr, radix));
}


int32_t OsKpl_Strtol(const int8_t *str, int8_t **endptr, int16_t radix)
{
	return ((int32_t) strtol(str, endptr, radix));
}


int8_t* OsKpl_Strcat(int8_t *s1, const int8_t *s2)
{
	return strcat(s1, s2);
}


int8_t* OsKpl_Strtok(int8_t *str, const int8_t *sep)
{
	return strtok(str, sep);
}


int8_t* OsKpl_Strncat(int8_t *s1, const int8_t *s2, uint32_t num)
{
	return strncat(s1, s2, num);
}


uint32_t OsKpl_Strlcat(int8_t *dst, const int8_t *src, uint32_t size)
{
	int8_t *d = dst;
	const int8_t *s = src;
	uint32_t n = size;
	uint32_t dlen;

	// Find the end of dst and adjust bytes left but don't go past end
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = (uint32_t) (d - dst);
	n = size - dlen;

	if (n == 0)
		return (dlen + OsKpl_Strlen(s));

	while (*s != '\0')
	{
		if (n != 1)
		{
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return (uint32_t) (dlen + (s - src));  // count does not include NULL
}


uint32_t OsKpl_Strlcpy(int8_t *dst, const int8_t *src, uint32_t size)
{
	int8_t *d = dst;
	const int8_t *s = src;
	uint32_t n = size;

	// Copy as many bytes as will fit
	if (n != 0)
	{
		while (--n != 0)
		{
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	// Not enough room in dst, add NUL and traverse rest of src
	if (n == 0)
	{
		if (size != 0)
			*d = '\0';		// NULL-terminate dst
		while (*s++)
			;
	}

	return (uint32_t) (s - src - 1);	// count does not include NULL
}


int8_t* OsKpl_Strstr(int8_t *mainString, int8_t *subString) {

	return strstr(mainString, subString);
}


int32_t OsKpl_Atoi(const int8_t *inStr) {
	return atoi(inStr);
}


int32_t OsKpl_Atol(const int8_t *inStr) {
	return atol(inStr);
}
