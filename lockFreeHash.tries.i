#include "lockFreeHash.tries.h"

#ifdef INCLUDE_DIC_LOCK_FREE_HASH_TRIE
/* nonpersistent macros - macros used inside the lfht model */
/* bench configuration - begin */
#define LFHT_STR                          struct dic
#define LFHT_STR_PTR                      dic_ptr
#define LFHT_USES_ARGS                    , long value
#define LFHT_PASS_ARGS                    , value
#define LFHT_ROOT_ADDR                    (&(Root.Root))
#define LFHT_FirstNode                    ((Root.Root))
#define LFHT_GetFirstNode(NODE)           (NODE = ((LFHT_STR_PTR) (Root.Root)))
#define LFHT_NodeKey(NODE)                Dic_key(NODE)
#define LFHT_NodeNext(NODE)               Dic_next(NODE)
#define LFHT_NEW_NODE(NODE, KEY, NEXT)    NEW_DIC_ENTRY(NODE, KEY, value, NEXT)
#define LFHT_FREE_NODE(PTR)               FREE_DIC_ENTRY(PTR)

#define LFHT_CHECK_INSERT_KEY             dic_check_insert_key
#define LFHT_CHECK_INSERT_FIRST_CHAIN     dic_check_insert_first_chain
#define LFHT_CHECK_INSERT_BUCKET_ARRAY    dic_check_insert_bucket_array
#define LFHT_CHECK_INSERT_BUCKET_CHAIN    dic_check_insert_bucket_chain
#define LFHT_ADJUST_CHAIN_NODES           dic_adjust_chain_nodes
#define LFHT_INSERT_BUCKET_ARRAY          dic_insert_bucket_array
#define LFHT_INSERT_BUCKET_CHAIN          dic_insert_bucket_chain
/* bench configuration - end */
/*-------------- don't change nothing from this point ------------------ */

static inline LFHT_STR_PTR LFHT_CHECK_INSERT_KEY(LFHT_NODE_KEY_STR key LFHT_USES_ARGS);
static inline LFHT_STR_PTR LFHT_CHECK_INSERT_FIRST_CHAIN(LFHT_STR_PTR chain_node, LFHT_NODE_KEY_STR key, int count_nodes LFHT_USES_ARGS);
static inline LFHT_STR_PTR LFHT_CHECK_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash,  LFHT_NODE_KEY_STR key, int n_shifts LFHT_USES_ARGS);
static inline LFHT_STR_PTR LFHT_CHECK_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, LFHT_STR_PTR chain_node,  LFHT_NODE_KEY_STR key, int n_shifts, int count_nodes  LFHT_USES_ARGS);
static inline void LFHT_ADJUST_CHAIN_NODES(LFHT_STR_PTR *new_hash, LFHT_STR_PTR chain_node, LFHT_STR_PTR last_node, int n_shifts LFHT_USES_REGS);
static inline void LFHT_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash, LFHT_STR_PTR chain_node, int n_shifts LFHT_USES_REGS);
static inline void LFHT_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, LFHT_STR_PTR chain_node, LFHT_STR_PTR adjust_node, int n_shifts, int count_nodes LFHT_USES_REGS);
#endif /* INCLUDE_DIC_LOCK_FREE_HASH_TRIE */

static inline LFHT_STR_PTR LFHT_CHECK_INSERT_KEY(LFHT_NODE_KEY_STR key LFHT_USES_ARGS) {
  LFHT_STR_PTR first_node;
  LFHT_GetFirstNode(first_node);
  if (first_node == NULL) {
    LFHT_STR_PTR new_node;
    LFHT_NEW_NODE(new_node, key, NULL);
    if (LFHT_BoolCAS(LFHT_ROOT_ADDR, NULL, new_node))
      return new_node;
    LFHT_FREE_NODE(new_node);
    LFHT_GetFirstNode(first_node);
  }
  if (LFHT_IsHashLevel(first_node))
    return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY((LFHT_STR_PTR *) first_node, key, 0);
  return LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(first_node, key, 0);
}

static inline LFHT_STR_PTR LFHT_CHECK_INSERT_FIRST_CHAIN(LFHT_STR_PTR chain_node, LFHT_NODE_KEY_STR key, int count_nodes LFHT_USES_ARGS) {
  if (LFHT_IsEqualKey(chain_node, key))
    return chain_node;  
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  chain_next = LFHT_NodeNext(chain_node); 
  if (chain_next && !LFHT_IsHashLevel(chain_next))
    return LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(chain_next, key, cn); 
  // chain_next refering the end of the chain or is a hash level pointer
  if (chain_next == NULL) {
    if (cn == LFHT_MAX_NODES_PER_BUCKET) {
      LFHT_STR_PTR *new_hash;
      LFHT_STR_PTR *bucket;                                     
      LFHT_AllocBuckets(new_hash, NULL, LFHT_STR);
      new_hash = (LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);
      LFHT_GetBucket(bucket, new_hash, LFHT_NodeKey(chain_node), 0, LFHT_STR);
      LFHT_SetBucket(bucket, chain_node, LFHT_STR);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), NULL, new_hash)) {
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, LFHT_FirstNode, chain_node, (- 1));
	LFHT_FirstNode = (LFHT_STR_PTR) new_hash;
	return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(new_hash, key, 0);
      } else
	LFHT_FreeBuckets(new_hash, bucket, LFHT_STR);
    } else {
      LFHT_STR_PTR new_node;
      LFHT_NEW_NODE(new_node, key, NULL);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), NULL, new_node))
	return new_node;
      LFHT_FREE_NODE(new_node);
    }
    chain_next = LFHT_NodeNext(chain_node);
    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(chain_next, key, cn);
  }
  // chain_next is refering a deeper hash level. The worker must jump its hash level
  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(jump_hash, key, 0);
}

static inline LFHT_STR_PTR LFHT_CHECK_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash,  LFHT_NODE_KEY_STR key, int n_shifts LFHT_USES_ARGS) {
  LFHT_STR_PTR *bucket;
  LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);
  if (LFHT_IsEmptyBucket(*bucket, curr_hash, LFHT_STR)) {
    LFHT_STR_PTR new_node;
    LFHT_NEW_NODE(new_node, key, (LFHT_STR_PTR) curr_hash);
    if (LFHT_BoolCAS(bucket, curr_hash, new_node))
      return new_node;
    LFHT_FREE_NODE(new_node);
  }
  LFHT_STR_PTR bucket_next = *bucket;
  if (LFHT_IsHashLevel(bucket_next))
    return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY((LFHT_STR_PTR *)bucket_next, key, (n_shifts + 1));
  return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash, bucket_next, key, n_shifts, 0);
}

static inline LFHT_STR_PTR LFHT_CHECK_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, LFHT_STR_PTR chain_node,  LFHT_NODE_KEY_STR key, int n_shifts, int count_nodes  LFHT_USES_ARGS) {
  if (LFHT_IsEqualKey(chain_node, key))
    return chain_node;
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  chain_next = LFHT_NodeNext(chain_node);
  if (!LFHT_IsHashLevel(chain_next))
    return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash, chain_next, key, n_shifts, cn);
  
  // chain_next is a hash pointer
  if ((LFHT_STR_PTR *)chain_next == curr_hash) {
    if (cn == LFHT_MAX_NODES_PER_BUCKET) {
      LFHT_STR_PTR *new_hash;
      LFHT_STR_PTR *bucket;
      LFHT_AllocBuckets(new_hash, curr_hash, LFHT_STR);
      new_hash = (LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);
      LFHT_GetBucket(bucket, new_hash, LFHT_NodeKey(chain_node), n_shifts + 1, LFHT_STR);
      LFHT_SetBucket(bucket, chain_node, LFHT_STR);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), curr_hash, new_hash)) {
	LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, *bucket, chain_node, n_shifts);
        LFHT_SetBucket(bucket, new_hash, LFHT_STR);
	return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(new_hash, key, (n_shifts + 1));
      } else 
	LFHT_FreeBuckets(new_hash, bucket, LFHT_STR);
    } else {
      LFHT_STR_PTR new_node;
      LFHT_NEW_NODE(new_node, key, (LFHT_STR_PTR) curr_hash);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), curr_hash, new_node))
	return new_node;
      LFHT_FREE_NODE(new_node);
    }
    chain_next = LFHT_NodeNext(chain_node);
    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash, chain_next, key, n_shifts, cn);
  }
  // chain_next is refering a deeper hash level. The worker must jump its hash level
  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash != curr_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(jump_hash, key, (n_shifts + 1));
}

static inline void LFHT_ADJUST_CHAIN_NODES(LFHT_STR_PTR *new_hash, LFHT_STR_PTR chain_node, LFHT_STR_PTR last_node, int n_shifts LFHT_USES_REGS) {
  if (chain_node == last_node)
    return;
  LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, LFHT_NodeNext(chain_node), last_node, n_shifts);
  return LFHT_CALL_INSERT_BUCKET_ARRAY(new_hash, chain_node, (n_shifts + 1));   
}

static inline void LFHT_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash, LFHT_STR_PTR chain_node, int n_shifts LFHT_USES_REGS) {
  LFHT_STR_PTR *bucket;
  LFHT_NodeNext(chain_node) = (LFHT_STR_PTR) curr_hash;
  LFHT_GetBucket(bucket, curr_hash, LFHT_NodeKey(chain_node), n_shifts, LFHT_STR);
  if (LFHT_IsEmptyBucket(*bucket, curr_hash, LFHT_STR))
    if (LFHT_BoolCAS(bucket, curr_hash, chain_node))
      return;
  LFHT_STR_PTR bucket_next = *bucket;
  if (LFHT_IsHashLevel(bucket_next))
    return LFHT_CALL_INSERT_BUCKET_ARRAY((LFHT_STR_PTR *)bucket_next, chain_node, (n_shifts + 1));
  return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, bucket_next, chain_node, n_shifts, 0);
}

static inline void LFHT_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, LFHT_STR_PTR chain_node, LFHT_STR_PTR adjust_node, int n_shifts, int count_nodes LFHT_USES_REGS) {
  LFHT_NODE_KEY_STR key = LFHT_NodeKey(adjust_node);
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  chain_next = LFHT_NodeNext(chain_node);
  if (!LFHT_IsHashLevel(chain_next))
    return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, chain_next, adjust_node, n_shifts, cn);
  // chain_next is a hash pointer
  if ((LFHT_STR_PTR *)chain_next == curr_hash) {
    if (cn == LFHT_MAX_NODES_PER_BUCKET) {
      LFHT_STR_PTR *new_hash;
      LFHT_STR_PTR *bucket;
      LFHT_AllocBuckets(new_hash, curr_hash, LFHT_STR);
      new_hash = (LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);
      LFHT_GetBucket(bucket, new_hash, LFHT_NodeKey(chain_node), n_shifts + 1, LFHT_STR);
      LFHT_SetBucket(bucket, chain_node, LFHT_STR);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), curr_hash, new_hash)) {
	LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, *bucket, chain_node, n_shifts);
        LFHT_SetBucket(bucket, new_hash, LFHT_STR);
	return LFHT_CALL_INSERT_BUCKET_ARRAY(new_hash, adjust_node, (n_shifts + 1));
      } else
	LFHT_FreeBuckets(new_hash, bucket, LFHT_STR);
    } else {
      LFHT_NodeNext(adjust_node) = (LFHT_STR_PTR) curr_hash;
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), curr_hash, adjust_node))
	return;
    }
    chain_next = LFHT_NodeNext(chain_node);
    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, chain_next, adjust_node, n_shifts, cn);
  }
  // chain_next is refering a deeper hash level. The worker must jump its hash level
  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash != curr_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_INSERT_BUCKET_ARRAY(jump_hash, adjust_node, (n_shifts + 1));
}

#undef LFHT_STR
#undef LFHT_STR_PTR
#undef LFHT_USES_ARGS
#undef LFHT_PASS_ARGS
#undef LFHT_ROOT_ADDR
#undef LFHT_NodeKey
#undef LFHT_NodeNext
#undef LFHT_GetFirstNode
#undef LFHT_FirstNode
#undef LFHT_NEW_NODE
#undef LFHT_FREE_NODE
#undef LFHT_CHECK_INSERT_KEY
#undef LFHT_CHECK_INSERT_FIRST_CHAIN
#undef LFHT_CHECK_INSERT_BUCKET_ARRAY
#undef LFHT_CHECK_INSERT_BUCKET_CHAIN
#undef LFHT_ADJUST_CHAIN_NODES
#undef LFHT_INSERT_BUCKET_ARRAY
#undef LFHT_INSERT_BUCKET_CHAIN

