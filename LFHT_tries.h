#ifndef _LFHT_TRIES_H
#define _LFHT_TRIES_H

#include "LFHT_common.h"
#include "LFHT_threads.h"

typedef enum {LFHT_false, LFHT_true} LFHT_Bool;

#define LFTH_LowTagDeleteKeyBits        1  /* 1 bit for the logically deleted key (for now) */
/*******************************************************************************
 *                            YapTab compatibility stuff                       *
 *******************************************************************************/
#ifdef YAP_TABMALLOC
/* persistent macros - macros used to call the lfht model from outside files */
#define LFHT_SUBGOAL_TRIE_CHECK_INSERT_ENTRY(K, P)   subgoal_trie_check_insert_key(K, P PASS_REGS)
#define LFHT_ANSWER_TRIE_CHECK_INSERT_ENTRY(K, P, I)   answer_trie_check_insert_key(K, P, I PASS_REGS)

/* 0 (zero) if none */
#define LFHT_NrLowTagBits                   LFTH_LowTagDeleteKeyBits + NumberOfLowTagBits
#define LFHT_DeleteKeyBit                   NumberOfLowTagBits
#define LFHT_USES_REGS                      , LFHT_ThreadEnvPtr LFHT_THREAD_ENV_PTR USES_REGS
#define LFHT_PASS_REGS                      , LFHT_THREAD_ENV_PTR PASS_REGS
#define LFHT_NODE_KEY_STR                   Term
#else  /* STANDARD_MALLOC */
#define LFHT_NrLowTagBits                   LFTH_LowTagDeleteKeyBits
#define LFHT_DeleteKeyBit                   0
#define LFHT_USES_REGS                      , LFHT_ThreadEnvPtr LFHT_THREAD_ENV_PTR
#define LFHT_PASS_REGS                      , LFHT_THREAD_ENV_PTR
#define LFHT_NODE_KEY_STR                   long
#endif /*YAP_TABMALLOC */

/*******************************************************************************
 *                            LFHT macros                                      *
 *******************************************************************************/
/* common macros - do not change*/

#define LFHT_NEW_NODE(NODE, KEY, NEXT, TENV)            \
  if ((NODE = LFHT_ThreadUnusedNode(TENV)) != NULL)	\
    LFHT_ThreadUnusedNode(TENV) = NULL;                 \
  else {					        \
    LFHT_ALLOC_NODE(NODE, KEY, NEXT);                   \
  }

#define LFHT_FREE_NODE(NODE, TENV)                       \
  if (LFHT_ThreadUnusedNode(TENV) == NULL) {	         \
    LFHT_NodeKey(NODE) = 0;  /* untag delete bit */	 \
    LFHT_ThreadUnusedNode(TENV) = NODE;                  \
  } else					         \
    LFHT_DEALLOC_NODE(NODE)

#define LFHT_CALL_CHECK_INSERT_KEY(K)                    \
  LFHT_CHECK_INSERT_KEY(K LFHT_PASS_ARGS)

#define LFHT_CALL_CHECK_DELETE_KEY(K)                    \
  LFHT_CHECK_DELETE_KEY(K LFHT_PASS_ARGS)

#define LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(F, K, S)      \
  LFHT_CHECK_INSERT_FIRST_CHAIN(F, K, S LFHT_PASS_ARGS)

#define LFHT_CALL_CHECK_DELETE_FIRST_CHAIN(F, K, S)      \
  LFHT_CHECK_DELETE_FIRST_CHAIN(F, K, S LFHT_PASS_ARGS)

#define LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(F, K, S)     \
  LFHT_CHECK_INSERT_BUCKET_ARRAY(F, K, S LFHT_PASS_ARGS)

#define LFHT_CALL_CHECK_DELETE_BUCKET_ARRAY(F, K, S)     \
  LFHT_CHECK_DELETE_BUCKET_ARRAY(F, K, S LFHT_PASS_ARGS)

#define LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(H, N, K, S, C) \
  LFHT_CHECK_INSERT_BUCKET_CHAIN(H, N, K, S, C LFHT_PASS_ARGS)

#define LFHT_CALL_CHECK_DELETE_BUCKET_CHAIN(H, N, K, S, C) \
  LFHT_CHECK_DELETE_BUCKET_CHAIN(H, N, K, S, C LFHT_PASS_ARGS)

#define LFHT_CALL_ADJUST_CHAIN_NODES(H, N, S)              \
  LFHT_ADJUST_CHAIN_NODES(H, N, S LFHT_PASS_REGS)

#define LFHT_CALL_INSERT_BUCKET_ARRAY(B, C, S)             \
  LFHT_INSERT_BUCKET_ARRAY(B, C, S LFHT_PASS_REGS)

#define LFHT_CALL_INSERT_BUCKET_CHAIN(H, N, L, S, C)       \
  LFHT_INSERT_BUCKET_CHAIN(H, N, L, S, C LFHT_PASS_REGS)

#define LFTH_ShiftDeleteBits(KEY)         ((KEY = KEY << LFTH_LowTagDeleteKeyBits))

#define LFTH_UnshiftDeleteBits(KEY)       ((KEY >> LFTH_LowTagDeleteKeyBits))

#define LFHT_TagAsDeletedKey(NODE)        \
  (LFHT_NodeKey(NODE) = LFHT_NodeKey(NODE) | (LFHT_CELL) 0x1)

#define LFHT_IsDeletedKey(NODE)            (LFHT_NodeKey(NODE) & (LFHT_CELL) 0x1)

#define LFHT_IsEqualKey(NODE, KEY)        (LFHT_NodeKey(NODE) == KEY)
#define LFHT_IsHashLevel(PTR)             ((LFHT_CELL)(PTR) & (LFHT_CELL)(0x1))
     /* V04_IS_EMPTY_BUCKET */
#define LFHT_IsEmptyBucket(B, H, STR)     (B == (STR *) H)
        /*v04_tag */
#define LFHT_TagAsHashLevel(PTR)          ((LFHT_CELL)(PTR) | (LFHT_CELL)0x1)
       /*v04_untag */
#define LFHT_UntagHashLevel(PTR)          ((LFHT_CELL)(PTR) & ~(LFHT_CELL)(0x1))
       /* V04_SET_HASH_BUCKET */
#define LFHT_SetBucket(BUCKET, V, STR)    (*(BUCKET) = (STR *) V)
     /* V04_SHIFT_ENTRY */
#define LFHT_ShiftKeyBits(KEY, NS)        \
  ((KEY) >> ((LFHT_BIT_SHIFT * NS) + LFHT_NrLowTagBits))

     /* V04_HASH_ENTRY */
#define LFHT_KeyOffset(KEY, NS)           \
  (LFHT_ShiftKeyBits(KEY, NS) & (LFHT_BUCKET_ARRAY_SIZE - 1))
      /* V04_GET_HASH_BUCKET */
#define LFHT_GetBucket(B, H, K, NS, STR)  \
  (B = (STR **) LFHT_UntagHashLevel(H) + LFHT_KeyOffset((LFHT_CELL)K, NS))
     /* V04_GET_PREV_HASH */
#define LFHT_GetPreviousHashLevel(PH, CH, STR)  \
  (PH = (STR **) *(((STR **) LFHT_UntagHashLevel(CH)) - 1))

/* Memory allocation stuff */

#ifdef YAP_TABMALLOC
#define LFHT_MemAllocBuckets(STR)                        		\
  union trie_hash_buckets *aux;						\
  ALLOC_STRUCT(aux, union trie_hash_buckets, _pages_trie_hash_buckets); \
  STR = aux->hash_buckets

#else /* STANDARD_MALLOC */

#define LFHT_MemAllocBuckets(PTR)                        		            \
  if ((PTR = (void **) malloc((LFHT_BUCKET_ARRAY_SIZE + 1)*sizeof(void *))) == NULL) \
    perror("ALLOC_DIC_ENTRY: malloc error")

#endif /* YAP_TABMALLOC */

#define LFHT_InitBuckets(BUCKET_PTR, PREV_HASH)                          \
  { int i; void **init_bucket_ptr;                                       \
    *BUCKET_PTR++ = (void *) (PREV_HASH);                                \
    init_bucket_ptr = (void *) BUCKET_PTR;                               \
    for (i = LFHT_BUCKET_ARRAY_SIZE; i != 0; i--)  {			 \
      /*printf("init bucket = %p \n",init_bucket_ptr); */		 \
      *init_bucket_ptr++ = (void **) LFHT_TagAsHashLevel(BUCKET_PTR);    \
    }						                         \
  }

#define LFHT_AllocBuckets(BUCKET_PTR, PREV_HASH, STR, TENV)		    \
  { void **alloc_bucket_ptr;						    \
    if ((alloc_bucket_ptr = LFHT_ThreadUnusedBucketArray(TENV)) != NULL)    \
      LFHT_ThreadUnusedBucketArray(TENV) = NULL;                            \
    else						                    \
      LFHT_MemAllocBuckets(alloc_bucket_ptr);			            \
    LFHT_InitBuckets(alloc_bucket_ptr, PREV_HASH);                          \
    BUCKET_PTR = (STR **) alloc_bucket_ptr;                                 \
  }

#define LFHT_FreeBuckets(PTR, TENV)                                        \
  LFHT_ThreadUnusedBucketArray(TENV) =                                     \
    (void **) (((LFHT_STR_PTR *)LFHT_UntagHashLevel(PTR)) - 1)

#ifdef YAP_TABMALLOC 
#define LFHT_DeallocateBucketArray(PTR)   
  /*  -- commented ... check this later */
  /* V04_FREE_THB(((STR *) V04_UNTAG(PTR)) - 1) 
     FREE_STRUCT((union trie_hash_buckets*)STR, union trie_hash_buckets, _pages_trie_hash_buckets)
     FREE_BLOCK(((ans_node_ptr *) V04_UNTAG(STR)) - 1) */

#else /* STANDARD_MALLOC */ 
#define LFHT_DeallocateBucketArray(PTR)			\
  free(((LFHT_STR_PTR *)LFHT_UntagHashLevel(PTR)) - 1)
#endif

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

struct LFHT_StatisticsCounters LFHT_Statistics;

#define LFHT_StatisticsResetGeneralCounters()       \
  LFHT_Statistics.nodes_valid =			    \
  LFHT_Statistics.bucket_used_entries =	            \
  LFHT_Statistics.bucket_empty_entries =	    \
  LFHT_Statistics.buckets_used =		    \
  LFHT_Statistics.nodes_deleted = 0
#endif /* _LFHT_TRIES_H */
