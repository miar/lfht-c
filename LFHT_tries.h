#ifndef _LFHT_TRIES_H
#define _LFHT_TRIES_H

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
#define LFHT_NrLowTagBits                              LFTH_LowTagDeleteKeyBits + NumberOfLowTagBits
#define LFHT_DeleteKeyBit                              NumberOfLowTagBits
#define LFHT_USES_REGS                                 USES_REGS
#define LFHT_PASS_REGS                                 PASS_REGS
#define LFHT_NODE_KEY_STR                              Term
#else  /* STANDARD_MALLOC */
#define LFHT_NrLowTagBits                              LFTH_LowTagDeleteKeyBits
#define LFHT_DeleteKeyBit                              0
#define LFHT_USES_REGS                                 
#define LFHT_PASS_REGS                                 
#define LFHT_NODE_KEY_STR                              long
#endif /*YAP_TABMALLOC */

/*******************************************************************************
 *                            LFHT macros                                      *
 *******************************************************************************/
/* common macros - do not change*/

#define LFHT_NEW_NODE(NODE, KEY, NEXT, TENV)    \
  if ((NODE = LFHT_UnusedNode(TENV)) != NULL)	\
    LFHT_UnusedNode(TENV) = NULL;               \
  else {					\
    LFHT_ALLOC_NODE(NODE, KEY, NEXT);           \
  }

#define LFHT_FREE_NODE(NODE, TENV)              \
    LFHT_UnusedNode(TENV) = NODE


#define LFHT_CALL_CHECK_INSERT_KEY(K)                      LFHT_CHECK_INSERT_KEY(K LFHT_PASS_ARGS)
#define LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(F, K, S)        LFHT_CHECK_INSERT_FIRST_CHAIN(F, K, S LFHT_PASS_ARGS)
#define LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(F, K, S)       LFHT_CHECK_INSERT_BUCKET_ARRAY(F, K, S LFHT_PASS_ARGS)
#define LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(H, N, K, S, C) LFHT_CHECK_INSERT_BUCKET_CHAIN(H, N, K, S, C LFHT_PASS_ARGS)
#define LFHT_CALL_ADJUST_CHAIN_NODES(H, N, L, S)           LFHT_ADJUST_CHAIN_NODES(H, N, L, S LFHT_PASS_REGS)
#define LFHT_CALL_INSERT_BUCKET_ARRAY(B, C, S)             LFHT_INSERT_BUCKET_ARRAY(B, C, S LFHT_PASS_REGS)
#define LFHT_CALL_INSERT_BUCKET_CHAIN(H, N, L, S, C)       LFHT_INSERT_BUCKET_CHAIN(H, N, L, S, C LFHT_PASS_REGS)

#define LFTH_ShiftDeleteBits(KEY)         ((KEY = KEY << LFTH_LowTagDeleteKeyBits))
#define LFTH_UnshiftDeleteBits(KEY)       ((KEY >> LFTH_LowTagDeleteKeyBits))

#define LFHT_TagAsDeletedKey(NODE)        (LFHT_NodeKey(NODE) = LFHT_NodeKey(NODE) | (LFHT_CELL) 0x1)

#define LFHT_IsDeletedKey(NODE)            (LFHT_NodeKey(NODE) | (LFHT_CELL) 0x1)

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
     /* LFHT_BOOL_CAS */
#define LFHT_BoolCAS(PTR, OLD, NEW)       __sync_bool_compare_and_swap((PTR), (OLD), (NEW))
     /* V04_SHIFT_ENTRY */
#define LFHT_ShiftKeyBits(KEY, NS)        ((KEY) >> ((LFHT_BIT_SHIFT * NS) + LFHT_NrLowTagBits))
     /* V04_HASH_ENTRY */
#define LFHT_KeyOffset(KEY, NS)           (LFHT_ShiftKeyBits(KEY, NS) & (LFHT_BUCKET_ARRAY_SIZE - 1))
      /* V04_GET_HASH_BUCKET */
#define LFHT_GetBucket(B, H, K, NS, STR)  (B = (STR **) LFHT_UntagHashLevel(H) + LFHT_KeyOffset((LFHT_CELL)K, NS))
     /* V04_GET_PREV_HASH */
#define LFHT_GetPreviousHashLevel(PH, CH, STR)  (PH = (STR **) *(((STR **) LFHT_UntagHashLevel(CH)) - 1))

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

#define LFHT_InitBuckets(BUCKET_PTR, PREV_HASH)                         \
  { int i; void **init_bucket_ptr;                                      \
    *BUCKET_PTR++ = (void *) (PREV_HASH);                               \
    init_bucket_ptr = (void *) BUCKET_PTR;                             \
    for (i = LFHT_BUCKET_ARRAY_SIZE; i != 0; i--)  {			\
      /*printf("init bucket = %p \n",init_bucket_ptr); */		\
      *init_bucket_ptr++ = (void **) LFHT_TagAsHashLevel(BUCKET_PTR);    \
    }						                        \
  }

#if defined(LFHT_LOCAL_THREAD_BUFFER_FOR_BUCKET_ARRAYS)
#define LFHT_LOCAL_BUCKET_BUFFER           LOCAL_trie_buckets_buffer

#define LFHT_AllocBuckets(BUCKET_PTR, PREV_HASH, STR)	    \
  { void **alloc_bucket_ptr;				    \
    if(LFHT_LOCAL_BUCKET_BUFFER == NULL) {		    \
      LFHT_MemAllocBuckets(alloc_bucket_ptr);		    \
      LFHT_InitBuckets(alloc_bucket_ptr, PREV_HASH);	    \
    } else {						    \
      alloc_bucket_ptr = LFHT_LOCAL_BUCKET_BUFFER;	    \
      LFHT_LOCAL_BUCKET_BUFFER = NULL;			    \
      *alloc_bucket_ptr++ = (void *) (PREV_HASH);	    \
    }							    \
    BUCKET_PTR = (STR **) alloc_bucket_ptr;	            \
  }
     /* V04_FREE_TRIE_HASH_BUCKETS */
#define LFHT_FreeBuckets(PTR, BKT, STR)                                     \
  { LFHT_SetBucket(BKT, PTR, STR);				            \
    LFHT_LOCAL_BUCKET_BUFFER = (((void**)LFHT_UntagHashLevel(PTR)) - 1);    \
  }

#else /* !LFHT_LOCAL_THREAD_BUFFER_FOR_BUCKET_ARRAYS */

#define LFHT_AllocBuckets(BUCKET_PTR, PREV_HASH, STR)	                             \
  { void **alloc_bucket_ptr;						             \
    LFHT_MemAllocBuckets(alloc_bucket_ptr);					     \
    LFHT_InitBuckets(alloc_bucket_ptr, PREV_HASH);                                   \
    BUCKET_PTR = (STR **) alloc_bucket_ptr;                                          \
  }

#ifdef YAP_TABMALLOC 
#define LFHT_FreeBuckets(PTR, BKT, STR)

  /* !! -- commented ... check this later */
  /* V04_FREE_THB(((STR *) V04_UNTAG(PTR)) - 1) 
     FREE_STRUCT((union trie_hash_buckets*)STR, union trie_hash_buckets, _pages_trie_hash_buckets)
     FREE_BLOCK(((ans_node_ptr *) V04_UNTAG(STR)) - 1) */

#else /* STANDARD_MALLOC */ 
#define LFHT_FreeBuckets(PTR, _NIU1_, _NIU2_)  \
  free(((LFHT_STR_PTR *)LFHT_UntagHashLevel(PTR)) - 1)
#endif

#endif /* LFHT_LOCAL_THREAD_BUFFER_FOR_BUCKET_ARRAYS */
#endif /* _LFHT_TRIES_H */
