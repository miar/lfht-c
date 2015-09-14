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
#define LFHT_ThreadNextRef(X)  ((X)->mem_ref)


typedef struct LFHT_ToDeleteNode {
  void *to_delete; /* only chain nodes for now */
  struct LFHT_ToDeleteNode *next;
} *LFHT_ToDeleteNodePtr;

#define LFHT_ToDelete(X)      ((X)->to_delete)
#define LFHT_ToDeleteNext(X)  ((X)->next)

typedef struct LFHT_Environment{
  LFHT_ToDeleteNodePtr to_delete_pool;            /* Data structures to delete pool*/
  LFHT_ThreadEnvPtr thread_pool[LFHT_MAX_THREADS];/* Thread pool */ 
} *LFHT_EnvPtr;

#define LFHT_DeletePool(X)           ((X)->to_delete_pool)
#define LFHT_ThreadEnv(X, Tid)       ((X)->thread_pool[Tid])

#define LFHT_InitEnv(LFHT_ENV)					                \
  { int i; 							  	        \
     LFHT_EnvPtr PTR;						                \
     if ((PTR = (LFHT_EnvPtr) malloc(sizeof(struct LFHT_Environment))) == NULL) \
       perror("Alloc LFHT Environment: malloc error");                          \
     LFHT_DeletePool(PTR) = NULL;					        \
     for (i = 0; i < LFHT_MAX_THREADS; i++)				        \
       LFHT_ThreadEnv(PTR, i) = NULL;				  	        \
     LFHT_ENV = PTR;							        \
  }

#define LFHT_InitThreadEnv(LFHT, Tid)	                                                    \
  {  LFHT_ThreadEnvPtr PTR;						                    \
     if ((PTR = (LFHT_ThreadEnvPtr) malloc(sizeof(struct LFHT_ThreadEnvironment))) == NULL) \
       perror("Alloc Thread Environment: malloc error");                                    \
     LFHT_ThreadMemRef(PTR) = LFHT_UnusedNode(PTR) = LFHT_UnusedBucket(PTR) = NULL;         \
     LFHT->thread_pool[Tid] = PTR;					                    \
  }

#endif  /* _LFHT_THREADS_H */
