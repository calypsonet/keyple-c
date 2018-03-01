/** 
Summary

*/

#ifndef __OS_KPL_STR_H
#define __OS_KPL_STR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* -------------------------------------------------------------------------- *
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#include "osKplCfg.h"

/* -------------------------------------------------------------------------- *
 * - Defines ---------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- *
 * - Types Definitions ------------------------------------------------------ *
 * -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- *
 * - Functions -------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

/**
 * Description
 * The OsKpl_Strlen() function calculates the length of the string inStr, not including
 * the terminating null (i.e. '\0') character.
 *
 * @param
 * inStr - Pointer to the string whose length is to be found.
 *
 * @return	
 * The OsKpl_Strlen() function returns the number of characters in inStr.
 */
uint32_t OsKpl_Strlen(const int8_t *inStr);


/**
 * Description
 * The OsKpl_Strcpy() function copies the string inSrc to inDest (including the terminating
 * null (i.e. '\0') character).
 *
 * @param
 * inDest - Pointer to the string where the content is to be copied.
 *
 * @param
 * inSrc - Pointer to the string to be copied.
 *
 * @return
 * The OsKpl_Strcpy() function returns a pointer to inDest.
 */
int8_t* OsKpl_Strcpy(int8_t *inDest, const int8_t *inSrc);


/**
 * Description
 * The OsKpl_Strdup() duplicates the string inStr. Memory for the new string is obtained
 * with OS_Malloc, and can be freed with OS_Free.
 *
 * @param
 * inStr - Pointer to the string to be duplicated.
 *
 * @return
 * The OsKpl_Strdup() function returns a pointer to a new string which is a duplicate
 * of the string inStr.
 *
 */
int8_t* OsKpl_Strdup(const int8_t *inStr);


/**
 * Description
 * The OsKpl_Strncpy() function copies at most inNumChars characters from inSrc into inDest.
 * If inSrc is less than inNumChars characters long, the remainder of inDest is filled with
 * null (i.e. '\0') characters.  Otherwise, inDest is not terminated.
 *
 * @param
 * inDest - Pointer to the string where the content is to be copied.
 *
 * @param
 * inSrc - Pointer to the string to be copied.
 *
 * @param
 * inNumChars - Number of characters to be copied from inSrc.
 *
 * @return	
 * The OsKpl_Strncpy() functions return a pointer to inDest.
 */
int8_t* OsKpl_Strncpy(int8_t *inDest, const int8_t *inSrc, uint32_t inNumChars);


/**
 * Description
 * The OsKpl_Strcmp() function compares the two strings inStr1 and inStr2.
 *
 * @param
 * inStr1 - Pointer to the first string to be compared.
 *
 * @param
 * inStr2 - Pointer to the second string to be compared.
 *
 * @return	
 * The OsKpl_Strcmp() function return an integer less than, equal to,
 * or greater than zero if inStr1 (or the first n bytes thereof) is found,
 * respectively, to be less than, to match, or be greater than inStr2.
 */
int32_t OsKpl_Strcmp(const int8_t *inStr1, const int8_t *inStr2);


/**
 * Description
 * The OsKpl_Strncmp() compares, at most, the first inNumChars characters of inStr1
 * and inStr2.
 *
 * @param
 * inStr1 - Pointer to the first string to be compared.
 *
 * @param
 * inStr2 - Pointer to the second string to be compared.
 *
 * @param
 * inNumChars - Number of characters to be copied from inSrc.
 *
 * @return	
 * The OsKpl_Strncmp() function return an integer less than, equal to,
 * or greater than zero if inStr1 (or the first inNumChars bytes thereof) is found,
 * respectively, to be less than, to match, or be greater than inStr2.
 */
int32_t OsKpl_Strncmp(const int8_t *s1, const int8_t *s2, uint32_t n);


/**
 * Description
 * The OsKpl_Strchr() function locates the first occurrence of inChar (converted to a char)
 * in the string inStr. If inC is '\0', strrchr() locates the terminating '\0'.
 *
 * @param
 * inStr - Pointer to the string where to search.
 *
 * @param
 * inChar - Character to be located. It is passed as its int16_t promotion, but it is 
 * internally converted back to char.
 *
 * @return	
 * The OsKpl_Strchr() function returns a pointer to the character, or a null
 * pointer if inChar does not occur anywhere in inStr.
 */
int8_t* OsKpl_Strchr(const int8_t *inStr, int16_t inChar);


/**
 * Description
 * The OsKpl_Strrchr() function locates the last occurrence of inChar (converted to a char)
 * in the string inStr.  If inChar is '\0', strrchr() locates the terminating '\0'.
 *
 * @param
 * inStr - Pointer to the string where to search.
 *
 * @param
 * inChar - Character to be located. It is passed as its int16_t promotion, but it is 
 * internally converted back to char.
 *
 * @return	
 * The OsKpl_Strrchr() function returns a pointer to the character, or a null
 * pointer if inChar does not occur anywhere in inStr.
 */
int8_t* OsKpl_Strrchr(const int8_t *inStr, int16_t inChar);


/**
 * Description
 * The OsKpl_StrReverse() function reverses the string.
 *
 * @param
 * inOriginal - Pointer to the string to reverse.
 *
 * @param
 * outReversed - Pointer to the reversed string.
 *
 * Remark
 * This is an internal function that is implemented internally and is not an OS native
 * function.
 *
 * @return	
 * The OsKpl_StrReverse() function returns an integer less than zero on error or zero if sucessful.
 */
int32_t OsKpl_StrReverse(int8_t *inOriginal, int8_t *outReversed);


/**
 * Description
 * The OsKpl_Strtoul() function converts the initial part of the string in inStr to an
 * unsigned long integer value according to the given base, which must be between
 * 2 and 36 inclusive, or be the special value 0.
 *
 * @param
 * inStr - String containing the representation of the number.
 *
 * @param
 * inEndptr - If inEndptr is not NULL, OsKpl_Strtoul() stores the address of the first
 * invalid character in inStr. If there are no digits at all OsKpl_Strtoul() stores the
 * original value of inStr in inEndptr (and returns 0).
 *
 * @param
 * inRadix - Base to be used in the conversion process.
 *
 * @return 
 * The OsKpl_Strtoul() function returns either the result of the conversion or,
 * if there was a leading minus sign, the negation of the result of the conversion
 * represented as an unsigned value, unless the original (non-negated) value would
 * overflow; in the latter case, OsKpl_Strtoul() returns ULONG_MAX and sets the global
 * variable errno to ERANGE.
 */
uint32_t OsKpl_Strtoul(const int8_t *inStr, int8_t **inEndptr, int16_t inRadix);


/**
 * Description
 * The OsKpl_Strtol() function converts the string in inStr to a long value.
 * The conversion is done according to the given base, which must be between 2
 * and 36 inclusive, or be the special value 0.
 *
 * @param
 * inStr - String containing the representation of the number.
 *
 * @param
 * inEndptr - If inEndptr is not NULL, OsKpl_Strtoul() stores the address of the first
 * invalid character in inStr. If there are no digits at all OsKpl_Strtoul() stores the
 * original value of inStr in inEndptr (and returns 0).
 *
 * @param
 * inRadix - Base to be used in the conversion process.
 *
 * @return 
 * The OsKpl_Strtol() function return the result of the conversion, unless
 * the value would underflow or overflow.
 * If no conversion could be performed, 0 is returned and the global vari-
 * able errno is set to EINVAL (the last feature is not portable across all
 * platforms).
 */
int32_t OsKpl_Strtol(const int8_t *inStr, int8_t **inEndptr, int16_t inRadix);


/**
 * Description
 * The OsKpl_Strcat() function appends a copy of the string pointed to by inSrc
 * (including the terminating null character) to the end of the string pointed to by inDest.
 * The initial character of inSrc overwrites the null character at the end of inDest.
 * If copying occurs between objects that overlap, the behavior is undefined.
 *
 * @param
 * inDest - Pointer to the destination string.
 *
 * @param
 * inSrc - Pointer to the string to be appended. This should not overlap destination.
 *
 * Remark
 * The function OsKpl_Strcat() does not allocate any storage. The caller must insure
 * that the buffer pointed to by inDest is long enough for string inSrc and its
 * terminating null character.
 *
 * @return	
 * The OsKpl_Strcat() function returns a pointer to inDest.
 */
int8_t* OsKpl_Strcat(int8_t *inDest, const int8_t *inSrc);


/**
 * Description
 * The OsKpl_Strtok() function is used to isolate sequential tokens in a null-terminated
 * string, inStr. These tokens are separated in the string by at least one of the 
 * characters in inDelim. 
 *
 * @param
 * inStr - Pointer to the string to be modified and broken into smaller strings (tokens).
 *
 * @param
 * inDelim - Pointer to the string containing the delimiters.
 *
 * Remark
 * The first time that OsKpl_Strtok() is called, inStr should be specified; subsequent 
 * calls, wishing to obtain further tokens from the same string, should pass a null 
 * pointer instead.
 * The separator string, inDelim, must be supplied each time, and may change between 
 * calls.
 *
 * @return 
 * The OsKpl_Strtok() function return a pointer to the beginning of each subsequent 
 * token in the string, after replacing the token itself with a NUL character. 
 * When no more tokens remain, a null pointer is returned.
 */
int8_t* OsKpl_Strtok(int8_t *inStr, const int8_t *inDelim);


/**
 * Description
 * The OsKpl_Strncat() function appends up to inNumChars characters from string inSrc to string
 * inDest, and then appends a terminating null character. The initial character of inSrc
 * overwrites the null character at the end of inDest. Subsequent characters in inSrc are
 * appended to inDest until either the end of inSrc is reached or inNumChars characters have 
 * been copied.
 * If copying takes place between objects that overlap, the behavior is undefined.
 *
 * @param
 * inDest - Pointer to the destination string.
 *
 * @param
 * inSrc - Pointer to the string to be appended. This should not overlap destination.
 *
 * @param
 * inNumChars - Maximum number of characters to be appended.
 *
 * Remark
 * The function OsKpl_Strncat() does not allocate any storage. The caller must insure
 * that the buffer pointed to by s1 is long enough to hold the added characters.
 *
 * @return	
 * The OsKpl_Strncat() function returns a pointer to inDest.
 */
int8_t* OsKpl_Strncat(int8_t *inDest, const int8_t *inSrc, uint32_t inNumChars);


/**
 * Description
 * The OsKpl_Strlcpy() and OsKpl_Strlcat() functions copy and concatenate strings
 * respectively. They are designed to be safer, more consistent, and less
 * error prone replacements for OsKpl_Strncpy and OsKpl_Strncat. Unlike those
 * functions, OsKpl_Strlcpy() and OsKpl_Strlcat() take the full size of the buffer (not
 * just the length) and guarantee to NULL-terminate the result (as long as
 * size is larger than 0 or, in the case of OsKpl_Strlcat(), as long as there is
 * at least one byte free in dst). Note that you should include a byte for
 * the NULL in size. Also note that OsKpl_Strlcpy() and OsKpl_Strlcat() only operate on
 * true 'C' strings. This means that for OsKpl_Strlcpy() src must be NULL-termi-
 * nated and for OsKpl_Strlcat() both src and dst must be NULL-terminated.
 *
 * The OsKpl_Strlcat() function appends the NULL-terminated string src to the end
 * of dst.  It will append at most size - OsKpl_Strlen(dst) - 1 bytes, NULL-termi-
 * nating the result.
 *
 * @return
 * The OsKpl_Strlcat() function return the total length of the string it tried
 * to create. That means the initial length of dst plus the length of
 * src. While this may seem somewhat confusing it was done to make trunca-
 * tion detection simple.
 *
 * Note however, that if OsKpl_Strlcat() traverses size characters without finding
 * a NULL, the length of the string is considered to be size and the destina-
 * tion string will not be NULL-terminated (since there was no space for the
 * NULL). This keeps OsKpl_Strlcat() from running off the end of a string. In
 * practice this should not happen (as it means that either size is incor-
 * rect or that dst is not a proper 'C' string). The check exists to pre-
 * vent potential security problems in incorrect code.
*/
uint32_t OsKpl_Strlcat(int8_t *inDest, const int8_t *inSrc, uint32_t inSize);


/**
 * Description
 * The OsKpl_Strlcpy() and OsKpl_Strlcat() functions copy and concatenate strings
 * respectively. They are designed to be safer, more consistent, and less
 * error prone replacements for OsKpl_Strncpy and OsKpl_Strncat. Unlike those
 * functions, OsKpl_Strlcpy() and OsKpl_Strlcat() take the full size of the buffer (not
 * just the length) and guarantee to NULL-terminate the result (as long as
 * size is larger than 0 or, in the case of OsKpl_Strlcat(), as long as there is
 * at least one byte free in dst). Note that you should include a byte for
 * the NULL in size. Also note that OsKpl_Strlcpy() and OsKpl_Strlcat() only operate on
 * true 'C' strings. This means that for OsKpl_Strlcpy() src must be NULL-termi-
 * nated and for OsKpl_Strlcat() both src and dst must be NULL-terminated.
 *
 * The OsKpl_Strlcpy() function copies up to size - 1 characters from the NULL-ter-
 * minated string src to dst, NULL-terminating the result.
 *
 * @return
 * The OsKpl_Strlcpy() function returns the total length of the string it tried
 * to create, that means the length of src.
 */
uint32_t OsKpl_Strlcpy(int8_t *inDest, const int8_t *inSrc, uint32_t inSize);


/**
 * Description
 * The OsKpl_Strstr() function locates the first occurrence of the null-terminated
 * string inSubString in the null-terminated string inMainString.
 *
 * @param
 * inMainString - Pointer to the string to be scanned.
 *
 * @param
 * inSubString - Pointer to the string to be searched within inMainString.
 *
 * @return
 * The OsKpl_Strstr() function returns a pointer to the first occurrence in inMainString
 * of any of the entire sequence of character specified in inSubString, or a null
 * pointer if the sequence is not present in mainString.
 */
int8_t* OsKpl_Strstr(int8_t *inMainString, int8_t *inSubString);


/**
 * Description
 * The OsKpl_Atoi() function parses the inStr string interpreting it content as an int.
 *
 * @param
 * inStr - String to be converted.
 *
 * @return
 * The OsKpl_Atoi() function returns the converted integral number as an int value.
 */
int32_t OsKpl_Atoi(const int8_t *inStr);


/**
 * Description
 * The OsKpl_Atol() function parses the inStr string interpreting it content as a long int.
 *
 * @param
 * inStr - String to be converted.
 *
 * @return
 * The OsKpl_Atol() function returns the converted integral number as a long int value.
 */
int32_t OsKpl_Atol(const int8_t *inStr);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __OS_KPL_STR_H
