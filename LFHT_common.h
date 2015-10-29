#ifndef _LFHT_COMMON_H
#define _LFHT_COMMON_H

typedef struct LFHT_ToDelete {
  void *node; /* only chain nodes for now */
  struct LFHT_ToDelete *next;
} *LFHT_ToDeletePtr;

#define LFHT_ToDeleteNode(X)  ((X)->node)
#define LFHT_ToDeleteNext(X)  ((X)->next)
#define LFHT_DeletePool       ((LFHT_ROOT_ENV)->to_delete_pool)

struct LFHT_StatisticsCounters {
  long nodes_valid;                  /* number of valid chain nodes */
#ifdef __NOT_IMPLEMENTED__
  long nodes_min_valid_per_chain;    /* non empty bucket entries */
  long nodes_avg_valid_per_chain;    /* non empty bucket entries */
  long nodes_max_valid_per_chain;    /* non empty bucket entries */  
#endif /* __NOT_IMPLEMENTED__ */
  long nodes_deleted;                /* number of deleted chain nodes */

#ifdef __NOT_IMPLEMENTED__
  long nodes_min_deleted_per_chain;  /* non empty bucket entries */
  long nodes_avg_deleted_per_chain;  /* non empty bucket entries */
  long nodes_max_deleted_per_chain;  /* non empty bucket entries */  

  long buckets_min_level;
  long buckets_avg_level;
  long buckets_max_level;
#endif /* __NOT_IMPLEMENTED__ */
  long bucket_used_entries;
  long bucket_empty_entries;
  long buckets_used;               /* total number of allocated and used buckets */

#ifdef LFHT_THREAD_STATISTICS
  long threads_nodes_allocated;    /* total number of allocated and used chain nodes */
  long threads_nodes_not_used;     /* total number of allocated and unused chain nodes */
  long threads_nodes_free;         /* number of freed chain nodes */
  long threads_buckets_not_used;   /* total number of allocated and unused buckets */

  long threads_delete_pool_attempts_to_enter;
  long threads_delete_pool_sucessfull_entries;
  long threads_delete_pool_visited_nodes;
  long threads_delete_pool_attempts_to_free_nodes;

  long threads_delete_pool_max_list_size;
  long threads_visited_nodes_valid;
  long threads_visited_nodes_deleted;
  long threads_visited_nodes_max_consecutive_valid;

  long threads_visited_nodes_max_consecutive_deleted;
  long threads_visited_buckets_max_consecutive;
  long threads_visited_buckets;
  long threads_accesses;

  long threads_keys_found;
  long threads_movements_to_previous_levels;
#endif /* LFHT_THREAD_STATISTICS */
};

#define LFHT_Statistics                 ((LFHT_ROOT_ENV)->statistics)

#define LFHT_StatisticsValidNodes                   \
  (LFHT_Statistics.nodes_valid)

#define LFHT_StatisticsDeletedNodes                 \
  (LFHT_Statistics.nodes_deleted)

#define LFHT_StatisticsUsedBucketEntries            \
  (LFHT_Statistics.bucket_used_entries)

#define LFHT_StatisticsEmptyBucketEntries           \
  (LFHT_Statistics.bucket_empty_entries)

#define LFHT_StatisticsUsedBucketArrayEntries       \
  (LFHT_Statistics.buckets_used)

#define LFHT_StatisticsResetGeneralCounters()       \
  LFHT_StatisticsValidNodes =			    \
  LFHT_StatisticsUsedBucketEntries =	            \
  LFHT_StatisticsEmptyBucketEntries =	            \
  LFHT_StatisticsUsedBucketArrayEntries =	    \
  LFHT_StatisticsDeletedNodes = 0

#define LFHT_BoolCAS(PTR, OLD, NEW)    __sync_bool_compare_and_swap((PTR), (OLD), (NEW))
#define LFHT_ValCAS(PTR, OLD, NEW)     __sync_val_compare_and_swap((PTR), (OLD), (NEW))

#endif /* _LFHT_COMMON_H */
