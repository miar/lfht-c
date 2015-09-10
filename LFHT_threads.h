#ifndef _LOCK_FREE_HASH_THREADS_H
#define _LOCK_FREE_HASH_THREADS_H

typedef struct LFTH_ThreadLocalData {
  void  *memory_reference;    /* Thread's current memory reference */
  void  *unused_node;         /* Thread's local buffer with an unused node */
  void **unused_bucketArray;  /* Thread's local buffer with an unused bucket array */
} *LFTH_ThreadLocalsPtr;

#define LFTH_ThreadLocals      struct LFTH_ThreadLocalData
#define LFTH_ThreadMemRef(X)   ((X)->memory_reference)
#define LFTH_UnusedNode(X)     ((X)->unused_node)
#define LFTH_UnusedBucket(X)   ((X)->unused_bucketArray)

#define LFTH_InitThreadLocals(Tid)                                                \
  { LFTH_ThreadLocalsPtr PTR;						          \
    if ((PTR = (LFTH_ThreadLocalsPtr) malloc(sizeof(LFTH_ThreadLocals))) == NULL) \
      perror("InitThreadLocals: malloc error") ;		    	          \
    LFTH_ThreadMemRef(PTR) = LFTH_UnusedNode(PTR) = UnusedBucket(PTR) = NULL;	  \
    LFTH_THREAD_POOL(Tid)= PTR;                                                   \
  }
#endif  /*_LOCK_FREE_HASH_THREADS_H */