/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2014 - 2016  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       emSSL * Embedded Transport Layer Security                    *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product for in-house use.         *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emSSL version: V2.54a                                        *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller GmbH
Licensed to:              Silicon Laboratories Inc., 400 W. Cesar Chavez, Austin, TX 78701, USA
Licensed SEGGER software: emSSL
License number:           SSL-00158
License model:            OEM LICENSE DISTRIBUTION AGREEMENT, signed June 3rd, 2018 and June 5th, 2018
----------------------------------------------------------------------
File    : SEGGER_MEM.h
Purpose : SEGGER memory allocator module header file.
Revision: $Rev: 11360 $
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SEGGER_MEM_H
#define SEGGER_MEM_H

#include "SEGGER.h"
#include "SEGGER_MEM_ConfDefaults.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define SEGGER_MEM_CanRealloc(pContext) (((pContext)->pAPI->pfRealloc == 0) ? 0 : 1)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  void*    (*pfAlloc)     (void* pContext, unsigned NumBytes);
  void     (*pfFree)      (void* pContext, void* pData);
  void*    (*pfRealloc)   (void* pContext, void* pData, unsigned NumBytes);  // Optional API pointer
  unsigned (*pfChunkSize) (void* pContext);
} SEGGER_MEM_API;

typedef struct {
  const SEGGER_MEM_API* pAPI;
        void*           pContext;
} SEGGER_MEM_CONTEXT;

typedef struct SEGGER_MEM_BUDDY_LINKS_tag SEGGER_MEM_BUDDY_LINKS;

struct SEGGER_MEM_BUDDY_LINKS_tag {
  SEGGER_MEM_BUDDY_LINKS* pNext;
  SEGGER_MEM_BUDDY_LINKS* pPrev;
};

typedef struct {
  U8   *  pStore;
  unsigned MaxK;     // 'm' in Fundamental Algorithms
  unsigned MinK;     // minimum block size returned
  unsigned MinSize;  // 1 << min_k
  unsigned Size;
  SEGGER_MEM_BUDDY_LINKS aFreeHead[sizeof(void* )*8];  // 4 entries unused, could do something here...
  //
#if SEGGER_MEM_STATS
  //
  // Statistics, only if configured.
  //
  struct {
    unsigned TotalCapacity;     // total size of the heap
    unsigned AllocRequests;     // number of times alloc called
    unsigned AllocFailures;     // how many times alloc has failed
    unsigned FreeRequests;      // number of times free called with non-zero argument
    unsigned FreeFailures;      // how many times free has been called with a bad argument or corrupt heap
    unsigned UsedBytes;         // how many bytes are used in the heap (a multiple of the allocation unit)
    unsigned FreeBytes;         // how many bytes are free in the heap
    unsigned MinFreeBytes;      // lowest number of free bytes bytes in the heap
    unsigned BlocksSplit;       // how many times blocks have needed to be split.
    unsigned BlocksMerged;      // how many times blocks have been coalesced.
  } Stats;
#endif
} SEGGER_MEM_BUDDY_HEAP;

typedef struct SEGGER_MEM_CHUNK_tag SEGGER_MEM_CHUNK;

typedef struct {
  unsigned          ChunkSize;
  void*             pBaseAddr;
  SEGGER_MEM_CHUNK* pFreeChunks;
#if SEGGER_MEM_STATS
  struct {
    unsigned NumAllocations;
    unsigned NumFrees;
    unsigned NumInUse;
    unsigned NumInUseMax;
  } Stats;
#endif
} SEGGER_MEM_CHUNK_HEAP;

typedef struct {
  unsigned          ChunkSize;
  unsigned          NumChunks;
  void*             pBaseAddr;
  SEGGER_MEM_CHUNK* pFreeChunks;
  struct {
    unsigned NumAllocations;
    unsigned NumFrees;
    unsigned NumInUse;
    unsigned NumInUseMax;
  } Stats;
} SEGGER_MEM_SELFTEST_HEAP;

typedef struct {
  SEGGER_MEM_CHUNK_HEAP SmallHeap;
  SEGGER_MEM_CONTEXT    SmallContext;
  SEGGER_MEM_CHUNK_HEAP LargeHeap;
  SEGGER_MEM_CONTEXT    LargeContext;
} SEGGER_MEM_DUAL_HEAP;

typedef struct {
  unsigned char*  pBuffer;
  unsigned        BufferSize;
  unsigned        NumBytesUsed;
  void*           pLastBlock;
#if SEGGER_MEM_STATS
  struct {
    unsigned NumBytesMax;
    unsigned NumAllocations;
    unsigned NumFrees;
  } Stats;
#endif
} SEGGER_MEM_SBUFFER;

typedef struct SEGGER_MEM_SIMPLE_HEAP_BLOCK_tag SEGGER_MEM_SIMPLE_HEAP_BLOCK;

struct SEGGER_MEM_SIMPLE_HEAP_BLOCK_tag {
  SEGGER_MEM_SIMPLE_HEAP_BLOCK* pNext;
  unsigned Size;
};

typedef struct {
  SEGGER_MEM_SIMPLE_HEAP_BLOCK* pHead;
  unsigned Size;
  unsigned ChunkSize;
#if SEGGER_MEM_STATS
  struct {
    long TotalCapacity;     // total size of the heap
    long AllocRequests;     // number of times alloc called
    long AllocFailures;     // how many times alloc has failed
    long FreeRequests;      // number of times free called with non-zero argument
    long FreeFailures;      // how many times free has been called with a bad argument or corrupt heap
    long UsedBytes;         // how many bytes are used in the heap (a multiple of the allocation unit)
    long FreeBytes;         // how many bytes are free in the heap
    long MinFreeBytes;      // lowest number of free bytes bytes in the heap
    long MaxUsedBytes;      // highest number of used bytes bytes in the heap
    long Fragmentation;     // number of non-contiguous free fragments (i.e. length of free list)
    long MaxFragmentation;  // maximum length of free list ever recorded
  } Stats;
#endif
} SEGGER_MEM_SIMPLE_HEAP;

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

void*    SEGGER_MEM_Alloc              (SEGGER_MEM_CONTEXT* pContext, unsigned NumBytes);
void*    SEGGER_MEM_ZeroAlloc          (SEGGER_MEM_CONTEXT* pContext, unsigned NumBytes);
void     SEGGER_MEM_Free               (SEGGER_MEM_CONTEXT* pContext, void* pMem);
void*    SEGGER_MEM_Realloc            (SEGGER_MEM_CONTEXT* pContext, void* pMem, unsigned NumBytes);
unsigned SEGGER_MEM_ChunkSize          (SEGGER_MEM_CONTEXT* pContext);

void     SEGGER_MEM_BUDDY_HEAP_Init    (SEGGER_MEM_CONTEXT* pMem, SEGGER_MEM_BUDDY_HEAP* pHeap, void* pStore, unsigned NumBytesStore);
void     SEGGER_MEM_CHUNK_HEAP_Init    (SEGGER_MEM_CONTEXT* pMem, SEGGER_MEM_CHUNK_HEAP* pHeap, void* pBuffer, unsigned NumChunks, unsigned ChunkSize);
void     SEGGER_MEM_SELFTEST_HEAP_Init (SEGGER_MEM_CONTEXT* pMem, SEGGER_MEM_SELFTEST_HEAP* pHeap, void* pBuffer, unsigned NumChunks, unsigned ChunkSize);
void     SEGGER_MEM_DUAL_HEAP_Init     (SEGGER_MEM_CONTEXT* pMem, SEGGER_MEM_DUAL_HEAP* pHeap, void* pSmallWorkspace, unsigned SmallNumChunks, unsigned SmallChunkSize, void* pLargeWorkspace, unsigned LargeNumChunks, unsigned LargeChunkSize);
void     SEGGER_MEM_SBUFFER_Init       (SEGGER_MEM_CONTEXT* pMem, SEGGER_MEM_SBUFFER* pSBuffer, void* pBuffer, unsigned BufferSize);
void     SEGGER_MEM_SIMPLE_HEAP_Init   (SEGGER_MEM_CONTEXT* pMem, SEGGER_MEM_SIMPLE_HEAP* pHeap, void* base, unsigned size, unsigned chunk_size);
void     SEGGER_MEM_SYSTEM_HEAP_Init   (SEGGER_MEM_CONTEXT* pMem);

void     SEGGER_MEM_Panic              (const char *sText);

#ifdef __cplusplus
}
#endif

#endif  // SEGGER_MEM_H

/****** End Of File* ************************************************/
