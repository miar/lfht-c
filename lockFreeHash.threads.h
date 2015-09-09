#ifndef _LOCK_FREE_HASH_THREADS_H
#define _LOCK_FREE_HASH_THREADS_H

#define LFHT_MAX_THREADS 1

typedef struct LFTH_ThreadLocalData {
  void *memory_reference;     /* Thread's current memory reference */
  LFHT_STR_PTR  unused_node;  /* Thread's local buffer with an unused node */
  void **unused_bucketArray;  /* Thread's local buffer with an unused bucket array of entries */
} *LFTH_ThreadLocals;

#define LFTH_ThreadMemRef(X)   ((X)->memory_reference)
#define LFTH_UnusedNode(X)     ((X)->unused_node)
#define LFTH_UnusedBucket(X)   ((X)->unused_bucketArray)

#endif  /*_LOCK_FREE_HASH_THREADS_H */
