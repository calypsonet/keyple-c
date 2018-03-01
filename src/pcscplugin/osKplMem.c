/** 
Summary

*/

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
#include "osKplMem.h"

/* -------------------------------------------------------------------------- * 
 * - Global Vars ------------------------------------------------------------ *
 * -------------------------------------------------------------------------- */

#ifdef K_OS_KPL_MEMORY_DEBUG
int32_t MallocCount = 0;
#endif

/* -------------------------------------------------------------------------- * 
 * - Internal Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- * 
 * - External Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

void* OsKpl_Realloc(void * inMemoryPointer, uint32_t inSize)
{
	#ifdef K_OS_KPL_MEMORY_DEBUG
	if( inMemoryPointer == NULL ) {
		MallocCount ++;
	}
	#endif

	return realloc(inMemoryPointer, inSize);
}



void* OsKpl_Malloc(uint32_t inSize)
{
	void * ptr = NULL;

	ptr = malloc(inSize);
	
	#ifdef K_OS_KPL_MEMORY_DEBUG
	if( ptr != NULL ) {
		MallocCount ++;
	}
	#endif

	return ptr;
}


void* OsKpl_Calloc(uint32_t inElementNumber, uint32_t inElementSize)
{
	void * ptr = NULL;
	

	ptr = calloc(inElementNumber, inElementSize);

	#ifdef K_OS_KPL_MEMORY_DEBUG
	if( ptr != NULL ) {
		MallocCount ++;
	}
	#endif

	return ptr;
}


void OsKpl_Free(void * inMemoryPointer)
{
	if( inMemoryPointer != NULL)
	{
		free(inMemoryPointer);

		#ifdef K_OS_KPL_MEMORY_DEBUG
		MallocCount --;
		#endif
	}
}


int32_t OsKpl_MemoryState(void)
{
#ifdef K_OS_KPL_MEMORY_DEBUG
	return MallocCount;
#else
	return 0;
#endif
}


void* OsKpl_Memset(void *outDestPointer, int16_t inValue, uint32_t inSize)
{
	if(inSize == 0) {
		return outDestPointer;
	}

	return memset(outDestPointer, inValue, inSize);
}



void* OsKpl_Memcpy(void *outDestPointer, void *inOrigPointer, uint32_t inSize)
{
	if(inSize == 0) {
		return outDestPointer;
	}
	
	return memcpy(outDestPointer, inOrigPointer, inSize);
}



int32_t OsKpl_Memcmp(void *inPointer1, void *inPointer2, uint32_t inSize)
{
	return ((int32_t)memcmp(inPointer1, inPointer2, inSize));
}

