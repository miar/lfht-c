#ifndef _LFHT_THREADS_H
#define _LFHT_THREADS_H

#include "LFHT_parameters.h" 

typedef struct LFHT_ThreadEnvironment {
  void  *mem_ref;             /* Thread's memory reference */
  void  *unused_node;         /* Thread's local buffer with an unused node */
  void **unused_bucketArray;  /* Thread's local buffer with an unused bucket array */
} *LFHT_ThreadEnvPtr;

#define LFHT_ThreadMemRef(X)   ((X)->mem_ref)
#define LFHT_UnusedNode(X)     ((X)->unused_node)
#define LFHT_UnusedBucket(X)   ((X)->unused_bucketArray)

typedef struct LFHT_ToDeleteNode {
  void *to_delete; /* only chain nodes for now */
  struct LFHT_ToDeleteNode *next;
} *LFHT_ToDeleteNodePtr;

#define LFHT_ToDelete(X)      ((X)->to_delete)
#define LFHT_ToDeleteNext(X)  ((X)->next)

typedef struct LFHT_Environment {
  LFHT_ToDeleteNodePtr *delete_pool;                            /* Data structures to delete */
  struct LFHT_ThreadEnvironment thread_pool[LFHT_MAX_THREADS];/* Thread pool */ 
} *LFHT_EnvPtr;

#define LFHT_DeletePool(X)           ((X)->delete_pool)
#define LFHT_ThreadEnv(X, Tid)       ((X)->thread_pool[Tid])

#define LFHT_Init(LFHT)					                \
  { int i; 								\
    LFHT_DeletePool(LFHT) = NULL;					\
    for (i = 0; i < LFHT_MAX_THREADS; i++)				\
      LFHT_ThreadEnv(LFHT, i) = NULL;					\
  }

/*
#define LFHT_InitThreadPool(LFHT, Tid)					\
  { LFHT_ThreadEnvPtr PTR;						                   \
    if ((PTR = (LFHT_ThreadEnvPtr) malloc(sizeof(struct LFHT_ThreadEnvironment))) == NULL) \
      perror("InitThreadLocals: malloc error") ;		    	                   \
    LFHT_ThreadMemRef(PTR) = LFHT_UnusedNode(PTR) = UnusedBucket(PTR) = NULL;	           \
    LFHT_THREAD_POOL(Tid)= PTR;                                                            \
  }

*/
#endif  /* _LFHT_THREADS_H */
