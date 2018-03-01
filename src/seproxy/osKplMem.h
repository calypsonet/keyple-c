/**
Summary

*/

#ifndef __OS_KPL_MEM_H
#define __OS_KPL_MEM_H


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
 * Implementation of the Realloc function that changes the size of the memory
 * object pointed to by inMemoryPointer to the size specified by inSize.
 *
 * @param 
 * inMemoryPointer - Pointer to the object being changed
 *
 * @param 
 * inSize - New size of the object
 *
 * @return 
 * Pointer to the allocated space.
 *
 */
void * OsKpl_Realloc(void * inMemoryPointer, uint32_t inSize);



/**
 * Description
 * Implementation of the Malloc function that allocates unused space for an
 * object whose size in bytes is specified by inSize and whose value is
 * unspecified.
 *
 * @param 
 * inSize - Size of the object
 *
 * @return 
 * Pointer to the allocated space.
 *
 */
void * OsKpl_Malloc(uint32_t inSize);


/**
 * Description
 * Implementation of the Calloc function that allocates unused space for an
 * array of inElementNumber elements each of whose size in bytes is inElementSize.
 * The space shall be initialized to all bits 0.
 *
 * @param 
 * inElementNumber - Number of elements
 *
 * @param 
 * inElementSize - Size of each element
 *
 * @return 
 * Pointer to the allocated space.
 *
 */
void * OsKpl_Calloc(uint32_t inElementNumber, uint32_t inElementSize);



/**
 * Description
 * Implementation of the Free function that causes the space pointed to by
 * inMemoryPointer to be deallocated; that is, made available for further
 * allocation.
 *
 * @param 
 * inMemoryPointer - Pointer to the object to be freed
 *
 */
void OsKpl_Free(void * inMemoryPointer);


/**
 * Description
 * Returns the number of the free operations missing to released all
 * allocated memory.
 * Note: Internal function for debug purposes.
 *
 * @return 
 * Number of dynamic memory objects allocated.
 *
 */
int32_t OsKpl_MemoryState(void);


/**
 * Description
 * Implementation of the Memset function that copies inValue (converted
 * to an unsigned char) into each of the first inSize bytes of the object
 * pointed to by outDestinationPointer.
 *
 * @param 
 * outDestinationPointer - Pointer to the object to changed
 *
 * @param 
 * inValue - Value to be set
 *
 * @param 
 * inSize - Number of bytes to be set
 *
 * @return 
 * Pointer to the changed buffer (=> outDestinationPointer).
 *
 */
void * OsKpl_Memset(void *outDestinationPointer, int16_t inValue, uint32_t inSize);



/**
 * Description
 * Implementation of the Memcpy function that copies copy inSize bytes from
 * the object pointed to by inOriginPointer into the object pointed to by
 * outDestinationPointer.
 *
 * @param 
 * outDestinationPointer - Pointer to the object to change
 *
 * @param 
 * inOriginPointer - Pointer to the object that contains the bytes to be copied
 *
 * @param 
 * inSize - Number of bytes to copy
 *
 * @return 
 * Pointer to the changed buffer (=> outDestinationPointer).
 *
 */
void * OsKpl_Memcpy(void *outDestinationPointer, void *inOriginPointer, uint32_t inSize);



/**
 * Description
 * Implementation of the Memcmp function that compare the first inSize bytes
 * (each interpreted as unsigned char) of the object pointed to by inPointer1
 * to the first inSize bytes of the object pointed to by inPointer2.
 *
 * @param 
 * inPointer1 - Pointer to the first object to be compared
 *
 * @param 
 * inPointer2 - Pointer to the second object to be compared
 *
 * @param 
 * inSize - Number of bytes to compare
 *
 * @return 
 * An integer greater than, equal to, or less than 0, if the object
 * pointed to by inPointer1 is greater than, equal to, or less than the object
 * pointed to by inPointer2, respectively.
 *
 */
int32_t OsKpl_Memcmp(void *inPointer1, void *inPointer2, uint32_t inSize);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __OS_KPL_MEM_H
