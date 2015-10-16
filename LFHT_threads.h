#ifndef _LFHT_THREADS_H
#define _LFHT_THREADS_H

#include "LFHT_parameters.h" 
#include "LFHT_common.h"

typedef struct LFHT_ThreadEnvironment {
  void  *mem_ref;               /* Thread's memory reference */
  void  *mem_ref_next;          /* Thread's next memory reference */
  void  *unused_node;           /* Thread's local buffer with an unused node */
  void **unused_bucketArray;    /* Thread's local buffer with an unused bucket array */
  //  long   state;                 /* 0 - unused and 1 - in use (Improve this later) */
  void  *cache_line_padding[4]; /* Cache line padding cache_line = 64 bytes */
} *LFHT_ThreadEnvPtr;

#define LFHT_ThreadMemRef(X)          ((X)->mem_ref)
#define LFHT_ThreadMemRefNext(X)      ((X)->mem_ref_next)
#define LFHT_UnusedNode(X)            ((X)->unused_node)
#define LFHT_UnusedBucketArray(X)     ((X)->unused_bucketArray)


typedef struct LFHT_ToDelete {
  void *node; /* only chain nodes for now */
  struct LFHT_ToDelete *next;
} *LFHT_ToDeletePtr;

#define LFHT_ToDeleteNode(X)  ((X)->node)
#define LFHT_ToDeleteNext(X)  ((X)->next)

typedef struct LFHT_Environment{
  LFHT_ToDeletePtr to_delete_pool;                /* Data structures to delete pool*/
  struct LFHT_ThreadEnvironment thread_pool[LFHT_MAX_THREADS];/* Thread pool */
} *LFHT_EnvPtr;

#define LFHT_DeletePool(X)           ((X)->to_delete_pool)
#define LFHT_ThreadEnv(X, Tid)       ((X)->thread_pool[Tid])

#define LFHT_SetThreadMemRefNext(TENV, MEM_REF)	  LFHT_ThreadMemRefNext(TENV) = MEM_REF
#define LFHT_UnsetThreadMemRefNext(TENV)	  LFHT_ThreadMemRefNext(TENV) = NULL;

#define LFHT_SetThreadMemRef(TENV, MEM_REF)	  LFHT_ThreadMemRef(TENV) = MEM_REF
#define LFHT_UnsetThreadMemRef(TENV)	          LFHT_ThreadMemRef(TENV) = NULL;


#define LFHT_InitEnv(LFHT_ENV)					                \
  {  LFHT_EnvPtr PTR;						                \
     if ((PTR = (LFHT_EnvPtr) malloc(sizeof(struct LFHT_Environment))) == NULL) \
       perror("Alloc LFHT Environment: malloc error");                          \
     LFHT_DeletePool(PTR) = NULL;					        \
     LFHT_ENV = PTR;							        \
  }

#define LFHT_KillThreadEnv(LFHT_ENV, Tid)                                       \
 { LFHT_ThreadEnvPtr PTR = &(LFHT_ThreadEnv(LFHT, Tid));                        \
   if (LFHT_UnusedNode(PTR) != NULL)                                            \
     LFHT_DEALLOC_NODE(LFHT_UnusedNode(PTR));                                   \
   if (LFHT_UnusedBucketArray(PTR) != NULL)                                     \
     LFHT_DeallocateBucketArray(LFHT_UnusedBucketArray(PTR));                   \
   LFHT_ThreadMemRef(PTR) = LFHT_ThreadMemRefNext(PTR) =                        \
     LFHT_UnusedNode(PTR) = LFHT_UnusedBucketArray(PTR) = NULL;                 \
 }

#define LFHT_InsertOnDeletePool(NODE)                                            \
 { LFHT_ToDeletePtr PTR;                                                         \
   if ((PTR = (LFHT_ToDeletePtr) malloc(sizeof(struct LFHT_ToDelete))) == NULL)  \
     perror("Alloc LFHT ToDelete node: malloc error");                           \
   LFHT_ToDeleteNode(PTR) = (void *) NODE;		                         \
   do {									         \
     LFHT_ToDeleteNext(PTR) = LFHT_DeletePool(LFHT_ROOT_ENV);		         \
   } while(!LFHT_BoolCAS((&(LFHT_DeletePool(LFHT_ROOT_ENV))),		         \
			LFHT_ToDeleteNext(PTR), PTR));                           \
 }


static inline
  LFHT_ThreadEnvPtr LFHT_InitThreadEnv(LFHT_EnvPtr LFHT,
				       int Tid) {
  LFHT_ThreadEnvPtr PTR = &(LFHT_ThreadEnv(LFHT, Tid));
  LFHT_ThreadMemRef(PTR) = LFHT_ThreadMemRefNext(PTR) = 
    LFHT_UnusedNode(PTR) = LFHT_UnusedBucketArray(PTR) = NULL;
  return PTR;
}


/* unused code - misc */

/* static inline  */
/*   void LFHT_InsDeletePool(void *node) { */

/*   LFHT_ToDeletePtr ptr; */
/*   if ((ptr = (LFHT_ToDeletePtr) malloc(sizeof(struct LFHT_ToDelete))) == NULL)  */
/*     perror("Alloc LFHT ToDelete node: malloc error"); */
/*   LFHT_ToDeleteNode(ptr) = node; */
/*   do { */
/*     LFHT_ToDeleteNext(ptr) = LFHT_DeletePool(LFHT_ROOT_ENV); */
/*   } while(LFHT_BoolCAS(&(LFHT_ToDeleteNext(ptr)),LFHT_DeletePool(LFHT_ROOT_ENV),ptr)); */
/*   return; */
/* } */

   /*  int i; */
   /* /\* optimize this search ...please *\/ */
   /*  for (i = 0; i < LFHT_MAX_THREADS; i++) { */
   /*    /\* pass all macros to this format ... instead of derrefing *\/ */

   /*    if (LFHT_ThreadEnv(LFHT_ROOT_ENV, i).mem_ref == chain_node || */
   /* 	  LFHT_ThreadEnv(LFHT_ROOT_ENV, i).mem_ref_next == chain_node) */
   /* 	/\* unable to free chain_node *\/ */
   /* 	return; */
   /*  } */
   /*  LFHT_FREE_NODE(chain_node, tenv); */




#endif  /* _LFHT_THREADS_H */
