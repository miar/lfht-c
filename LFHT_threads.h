#ifndef _LFHT_THREADS_H
#define _LFHT_THREADS_H

#include "LFHT_parameters.h" 
#include "LFHT_common.h"

typedef struct LFHT_ThreadEnvironment {
  void  *mem_ref;               /* Thread's memory reference */
  void  *mem_ref_next;          /* Thread's next memory reference */
  void  *unused_node;           /* Thread's local buffer with an unused node */
  void **unused_bucketArray;    /* Thread's local buffer with an unused bucket array */
  long   number_of_operations;  /* Number os insert / delete key operations done */
  void  *cache_line_padding[3]; /* Cache line padding cache_line = 64 bytes */
} *LFHT_ThreadEnvPtr;

#define LFHT_ThreadMemRef(X)             ((X)->mem_ref)
#define LFHT_ThreadMemRefNext(X)         ((X)->mem_ref_next)
#define LFHT_ThreadUnusedNode(X)         ((X)->unused_node)
#define LFHT_ThreadUnusedBucketArray(X)  ((X)->unused_bucketArray)
#define LFHT_ThreadNrOfOps(X)            ((X)->number_of_operations)

typedef struct LFHT_Environment{
  struct LFHT_StatisticsCounters statistics;  /* LFHT_common.h */
  LFHT_ToDeletePtr to_delete_pool;            /* LFHT_common.h */    
  struct LFHT_ThreadEnvironment thread_pool[LFHT_MAX_THREADS];
} *LFHT_EnvPtr;


#define LFHT_ThreadEnv(X, Tid)       ((X)->thread_pool[Tid])

#define LFHT_SetThreadMemRefNext(TENV, MEM_REF)	  LFHT_ThreadMemRefNext(TENV) = MEM_REF
#define LFHT_UnsetThreadMemRefNext(TENV)	  LFHT_ThreadMemRefNext(TENV) = NULL

#define LFHT_SetThreadMemRef(TENV, MEM_REF)	  LFHT_ThreadMemRef(TENV) = MEM_REF
#define LFHT_UnsetThreadMemRef(TENV)	          LFHT_ThreadMemRef(TENV) = NULL


#endif  /* _LFHT_THREADS_H */
