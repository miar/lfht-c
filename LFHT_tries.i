#ifdef INCLUDE_DIC_LOCK_FREE_HASH_TRIE
/* nonpersistent macros - macros used inside the lfht model */

/* Define the calls made by your bechmark */
#define LFHT_THREAD_ENV_PTR               tenv

#define LFHT_ROOT_ENV                     Root.dic_env

#define LFHT_STR                          struct dic

#define LFHT_STR_PTR                      dic_ptr

#define LFHT_USES_ARGS                    \
  , long value, LFHT_ThreadEnvPtr LFHT_THREAD_ENV_PTR

#define LFHT_PASS_ARGS                    \
  , value, LFHT_THREAD_ENV_PTR

#define LFHT_ROOT_ADDR                    (&(Root.dic))

#define LFHT_FirstNode                    ((Root.dic))

#define LFHT_GetFirstNode(NODE)           (NODE = ((LFHT_STR_PTR) (Root.dic)))

#define LFHT_NodeKey(NODE)                Dic_key(NODE)

#define LFHT_NodeNext(NODE)               Dic_next(NODE)

#define LFHT_ALLOC_NODE(NODE, KEY, NEXT)  NEW_DIC_ENTRY(NODE, KEY, value, NEXT)

#define LFHT_DEALLOC_NODE(NODE)           FREE_DIC_ENTRY(NODE)

#ifdef LFHT_DEBUG

#define LFHT_SHOW_TO_DELETE_NODE(NODE)                                \
  SHOW_DIC_ENTRY(NODE, LFTH_UnshiftDeleteBits(LFHT_NodeKey(NODE)));   \
  printf(" F\n"); /* Free */


#define LFHT_SHOW_NODE(NODE)                                          \
  SHOW_DIC_ENTRY(NODE, LFTH_UnshiftDeleteBits(LFHT_NodeKey(NODE)));   \
  if (LFHT_IsDeletedKey(NODE))				              \
    printf(" D\n"); /* Deleted */				      \
  else								      \
    printf(" V\n")  /* Valid */

#else /* !LFHT_DEBUG */

#define LFHT_SHOW_TO_DELETE_NODE(NODE)

#define LFHT_SHOW_NODE(NODE)                                          \
  SHOW_DIC_ENTRY(NODE, LFTH_UnshiftDeleteBits(LFHT_NodeKey(NODE)));   \
  printf("\n")

#endif /* LFHT_DEBUG */


#define LFHT_CHECK_INSERT_KEY               dic_check_insert_key
#define LFHT_CHECK_DELETE_KEY               dic_check_delete_key
#define LFHT_CHECK_INSERT_FIRST_CHAIN       dic_check_insert_first_chain
#define LFHT_CHECK_DELETE_FIRST_CHAIN       dic_check_delete_first_chain
#define LFHT_CHECK_INSERT_BUCKET_ARRAY      dic_check_insert_bucket_array
#define LFHT_CHECK_DELETE_BUCKET_ARRAY      dic_check_delete_bucket_array
#define LFHT_CHECK_INSERT_BUCKET_CHAIN      dic_check_insert_bucket_chain
#define LFHT_CHECK_DELETE_BUCKET_CHAIN      dic_check_delete_bucket_chain
#define LFHT_ADJUST_CHAIN_NODES             dic_adjust_chain_nodes
#define LFHT_INSERT_BUCKET_ARRAY            dic_insert_bucket_array
#define LFHT_INSERT_BUCKET_CHAIN            dic_insert_bucket_chain
#define LFHT_SHOW_STATE                     dic_show_state
#define LFHT_SHOW_CHAIN                     dic_show_chain
#define LFHT_SHOW_BUCKET_ARRAY              dic_show_bucket_array
#define LFHT_SHOW_STATISTICS                dic_show_statistics
#define LFHT_SHOW_STATISTICS_CHAIN          dic_show_statistics_chain
#define LFHT_SHOW_STATISTICS_BUCKET_ARRAY   dic_show_statistics_bucket_array
#define LFHT_ABOLISH_ALL_KEYS               dic_abolish_all_keys
#define LFHT_ABOLISH_CHAIN                  dic_abolish_chain
#define LFHT_ABOLISH_BUCKET_ARRAY           dic_abolish_bucket_array

/*-------------- don't change nothing from this point ------- */

static inline LFHT_STR_PTR 
  LFHT_CHECK_INSERT_KEY(LFHT_NODE_KEY_STR key 
			LFHT_USES_ARGS);

static inline LFHT_STR_PTR 
  LFHT_CHECK_DELETE_KEY(LFHT_NODE_KEY_STR key 
                        LFHT_USES_ARGS);

static inline LFHT_STR_PTR 
  LFHT_CHECK_INSERT_FIRST_CHAIN(LFHT_STR_PTR chain_node, 
				LFHT_NODE_KEY_STR key, 
				int count_nodes 
				LFHT_USES_ARGS);

static inline LFHT_STR_PTR 
  LFHT_CHECK_DELETE_FIRST_CHAIN(LFHT_STR_PTR chain_node,
				LFHT_NODE_KEY_STR key,
				int count_nodes
				LFHT_USES_ARGS);

static inline LFHT_STR_PTR 
  LFHT_CHECK_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash,  
				 LFHT_NODE_KEY_STR key, 
				 int n_shifts 
				 LFHT_USES_ARGS);

static inline LFHT_STR_PTR 
  LFHT_CHECK_DELETE_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash,  
				 LFHT_NODE_KEY_STR key, 
				 int n_shifts 
				 LFHT_USES_ARGS);

static inline LFHT_STR_PTR 
  LFHT_CHECK_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash,
				 LFHT_STR_PTR chain_node,
				 LFHT_NODE_KEY_STR key, 
				 int n_shifts, 
				 int count_nodes 
				 LFHT_USES_ARGS);
static inline LFHT_STR_PTR 
  LFHT_CHECK_DELETE_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash,
				 LFHT_STR_PTR chain_node,
				 LFHT_NODE_KEY_STR key, 
				 int n_shifts, 
				 int count_nodes 
				 LFHT_USES_ARGS);

static inline void 
  LFHT_ADJUST_CHAIN_NODES(LFHT_STR_PTR *new_hash, 
			  LFHT_STR_PTR chain_node, 
			  int n_shifts
			  LFHT_USES_REGS);

static inline void 
  LFHT_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash, 
			   LFHT_STR_PTR chain_node, 
			   int n_shifts 
			   LFHT_USES_REGS);

static inline void 
  LFHT_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, 
			   LFHT_STR_PTR chain_node, 
			   LFHT_STR_PTR adjust_node, 
			   int n_shifts, 
			   int count_nodes 
			   LFHT_USES_REGS);

static inline void 
  LFHT_SHOW_STATE(void);

static inline void 
  LFHT_SHOW_CHAIN(LFHT_STR_PTR chain_node, 
		  LFHT_STR_PTR * end_chain);

static inline void 
  LFHT_SHOW_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash);

static inline void 
  LFHT_SHOW_STATISTICS_STATE(void);

static inline void 
  LFHT_SHOW_STATISTICS_CHAIN(LFHT_STR_PTR chain_node, 
		  LFHT_STR_PTR * end_chain);

static inline void 
  LFHT_SHOW_STATISTICS_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash);

static inline void 
  LFHT_ABOLISH_ALL_KEYS(void);

static inline void 
  LFHT_ABOLISH_CHAIN(LFHT_STR_PTR chain_node, 
		     LFHT_STR_PTR * end_chain);

static inline void 
  LFHT_ABOLISH_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash);

#endif /* INCLUDE_DIC_LOCK_FREE_HASH_TRIE */

/* ----------------------------------------------------------*/
/*                      To Delete Pool Stuff                 */
/* ----------------------------------------------------------*/

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



#define LFHT_ShowDeletePool()					      \
  { LFHT_ToDeletePtr PTR = LFHT_DeletePool(LFHT_ROOT_ENV);	      \
    while (PTR) {						      \
       LFHT_SHOW_NODE((LFHT_STR_PTR) LFHT_ToDeleteNode(PTR));         \
      PTR = LFHT_ToDeleteNext(PTR);			              \
    }								      \
  }

static inline
  void LFHT_FreeToDeletePool(void); 


/* ----------------------------------------------------------*/
/*                      check_insert                         */
/* ----------------------------------------------------------*/

static inline LFHT_STR_PTR 
  LFHT_CHECK_INSERT_KEY(LFHT_NODE_KEY_STR key 
			LFHT_USES_ARGS) {
  LFTH_ShiftDeleteBits(key);
 LFHT_CHECK_INSERT_KEY:
  LFHT_GetFirstNode(LFHT_ThreadMemRef(tenv));

  if (LFHT_ThreadMemRef(tenv) == NULL) {
    LFHT_STR_PTR new_node;
    LFHT_NEW_NODE(new_node, key, NULL, tenv);
    LFHT_ThreadMemRef(tenv) = new_node;

    if (LFHT_BoolCAS(LFHT_ROOT_ADDR, NULL, new_node)) {
#ifdef LFHT_DEBUG
      total_nodes++;
#endif /* LFHT_DEBUG */
      return new_node;
    }
    LFHT_FREE_NODE(new_node, tenv);
    LFHT_GetFirstNode(LFHT_ThreadMemRef(tenv));
    if (LFHT_ThreadMemRef(tenv) == NULL)
      /* Thread has not inserted its key, otherwise the 
	 lastest "if" would have been true.  However thread 
         can read NULL since LFHT first node might have been 
         removed meanwhile. In this case thread must go back 
	 to its previous state. */      
      goto LFHT_CHECK_INSERT_KEY;
  }

  LFHT_STR_PTR node;
  if (LFHT_IsHashLevel(LFHT_ThreadMemRef(tenv)))
    node = LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(
            (LFHT_STR_PTR *) LFHT_ThreadMemRef(tenv),  key, 0);
  else
    node = LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(LFHT_ThreadMemRef(tenv), key, 0);

  if (++LFHT_ThreadNrOfOps(tenv) == LFHT_FREE_TO_DELETE_POOL) {
    LFHT_ThreadNrOfOps(tenv) = 0;
    LFHT_FreeToDeletePool();
  }
  
  return node;
}

/*
static inline LFHT_STR_PTR LFHT_CHECK_INSERT_KEY_ORIGINAL(LFHT_NODE_KEY_STR key LFHT_USES_ARGS) {
  LFTH_ShiftDeleteBits(key);
  LFHT_STR_PTR first_node;
  LFHT_GetFirstNode(first_node);
  if (first_node == NULL) {
    LFHT_STR_PTR new_node;
    LFHT_NEW_NODE(new_node, key, NULL, tenv);
    //    if (key == 2) {
    //  LFHT_TagAsDeletedKey(new_node);     
    ///}

    if (LFHT_BoolCAS(LFHT_ROOT_ADDR, NULL, new_node)) {
      return new_node;
    }
    LFHT_FREE_NODE(new_node, tenv);
    LFHT_GetFirstNode(first_node);
  }
  if (LFHT_IsHashLevel(first_node))
    return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY((LFHT_STR_PTR *) first_node, key, 0);
  return LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(first_node, key, 0);
}
*/

static inline LFHT_STR_PTR 
  LFHT_CHECK_INSERT_FIRST_CHAIN(LFHT_STR_PTR chain_node, 
				LFHT_NODE_KEY_STR key, 
				int count_nodes 
				LFHT_USES_ARGS) {
  /* Don't forget that:
     at this point LFHT_ThreadMemRef(tenv) = chain_node 
     tenv is hidden in LFHT_USES_ARGS */
  if (LFHT_IsEqualKey(chain_node, key))
    return chain_node;  
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  /* thread is still working in chain_node */
  chain_next = 
    LFHT_ThreadMemRefNext(tenv) = LFHT_NodeNext(chain_node);

  if (chain_next && !LFHT_IsHashLevel(chain_next)) {
    LFHT_ThreadMemRef(tenv) = chain_next;
    LFHT_UnsetThreadMemRefNext(tenv); // this might be useless !!
    return LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(chain_next, 
	     key, cn); 
  }
  /* chain_next refering the end of the chain or 
     is a hash level pointer */
  if (chain_next == NULL) {
    if (cn == LFHT_MAX_NODES_PER_BUCKET) {
      LFHT_STR_PTR *new_hash;
      LFHT_AllocBuckets(new_hash, NULL, LFHT_STR, tenv);
      new_hash = 
	(LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);

      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), NULL, 
		       (LFHT_STR_PTR) new_hash)) {
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, LFHT_FirstNode, (- 1));
	LFHT_FirstNode = (LFHT_STR_PTR) new_hash;
	return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(new_hash, 
		key, 0);
      } else {
	LFHT_FreeBuckets(new_hash, tenv);
      }
    } else {
      LFHT_STR_PTR new_node;
      LFHT_NEW_NODE(new_node, key, NULL, tenv);
      LFHT_ThreadMemRefNext(tenv) = new_node;
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), 
		       NULL, new_node)) {
#ifdef LFHT_DEBUG
	total_nodes++;
#endif /* LFHT_DEBUG */
	return new_node;
      }
      LFHT_FREE_NODE(new_node, tenv);
    }
    /* thread is leaving the chain_node */
    chain_next = 
      LFHT_ThreadMemRef(tenv) = LFHT_NodeNext(chain_node);

    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(chain_next, 
	       key, cn);
  }
  /* chain_next is refering a deeper hash level. 
     Thread must jump a previous hash level */
  LFHT_UnsetThreadMemRef(tenv);
  LFHT_UnsetThreadMemRefNext(tenv);

  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }

  return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(jump_hash, key, 0);
}

/*
static inline LFHT_STR_PTR LFHT_CHECK_INSERT_FIRST_CHAIN_ORIGINAL(LFHT_STR_PTR chain_node, LFHT_NODE_KEY_STR key, int count_nodes LFHT_USES_ARGS) {
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
      LFHT_AllocBuckets(new_hash, NULL, LFHT_STR, tenv);
      new_hash = (LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);
      LFHT_GetBucket(bucket, new_hash, LFHT_NodeKey(chain_node), 0, LFHT_STR);
      //      printf ("adjust first chain bucket %p", bucket);
      //SHOW_DIC_ENTRY(chain_node);
      LFHT_SetBucket(bucket, chain_node, LFHT_STR);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), NULL, (LFHT_STR_PTR) new_hash)) {
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, LFHT_FirstNode, chain_node, (- 1));
	LFHT_FirstNode = (LFHT_STR_PTR) new_hash;
	return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(new_hash, key, 0);
      } else
	LFHT_FreeBuckets(new_hash, tenv);
    } else {
      LFHT_STR_PTR new_node;
      LFHT_NEW_NODE(new_node, key, NULL, tenv);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), NULL, new_node)) {
	total_nodes++;
	return new_node;
      }
      LFHT_FREE_NODE(new_node, tenv);
    }
    chain_next = LFHT_NodeNext(chain_node);
    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_CHECK_INSERT_FIRST_CHAIN(chain_next, key, cn);
  }
  // chain_next is refering a deeper hash level. Thread must jump its hash level
  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(jump_hash, key, 0);
}

*/

static inline LFHT_STR_PTR 
  LFHT_CHECK_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash,  
				 LFHT_NODE_KEY_STR key, 
				 int n_shifts 
				 LFHT_USES_ARGS) {

  /* Don't forget that at this point 
     LFHT_ThreadMemRef(tenv) = curr_hash */
  LFHT_STR_PTR *bucket;
  LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);

  if (LFHT_IsEmptyBucket(*bucket, curr_hash, LFHT_STR)) {
    LFHT_STR_PTR new_node;
    LFHT_NEW_NODE(new_node, key, (LFHT_STR_PTR) curr_hash,
      tenv);

    LFHT_ThreadMemRefNext(tenv) = new_node;

    if (LFHT_BoolCAS(bucket, (LFHT_STR_PTR) curr_hash, 
		     new_node)) {
#ifdef LFHT_DEBUG
      total_nodes++;
#endif /* LFHT_DEBUG */
      return new_node;
    }
    LFHT_FREE_NODE(new_node, tenv);
  }
  LFHT_STR_PTR bucket_next =
    LFHT_ThreadMemRef(tenv) = *bucket;

  if (LFHT_IsHashLevel(bucket_next))
    return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(
             (LFHT_STR_PTR *)bucket_next, key, (n_shifts + 1));

  return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash, 
           bucket_next, key, n_shifts, 0);
}

/*
static inline LFHT_STR_PTR LFHT_CHECK_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash,  LFHT_NODE_KEY_STR key, int n_shifts LFHT_USES_ARGS) {
  LFHT_STR_PTR *bucket;
  LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);
  //  if (key == 5)
    //    printf("3- bucket=%p n_shifts = %d hash= %p", bucket, n_shifts,curr_hash);
  if (LFHT_IsEmptyBucket(*bucket, curr_hash, LFHT_STR)) {
    LFHT_STR_PTR new_node;
    LFHT_NEW_NODE(new_node, key, (LFHT_STR_PTR) curr_hash, tenv);
    if (LFHT_BoolCAS(bucket, (LFHT_STR_PTR) curr_hash, new_node)) {
#ifdef LFHT_DEBUG
      //      printf("3- bucket=%p n_shifts = %d hash= %p", bucket, n_shifts,curr_hash);
      //SHOW_DIC_ENTRY(new_node);
      total_nodes++;
#endif // LFHT_DEBUG
      return new_node;
    }
    LFHT_FREE_NODE(new_node, tenv);
  }
  LFHT_STR_PTR bucket_next = *bucket;
  if (LFHT_IsHashLevel(bucket_next))
    return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY((LFHT_STR_PTR *)bucket_next, key, (n_shifts + 1));
  return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash, bucket_next, key, n_shifts, 0);
}

*/

static inline LFHT_STR_PTR 
  LFHT_CHECK_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, 
				 LFHT_STR_PTR chain_node,  
				 LFHT_NODE_KEY_STR key, 
				 int n_shifts, 
				 int count_nodes  
				 LFHT_USES_ARGS) {

  /* Don't forget that at this point 
     LFHT_ThreadMemRef(tenv) = chain_node */

  if (LFHT_IsEqualKey(chain_node, key))
    return chain_node;
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  chain_next = 
    LFHT_ThreadMemRefNext(tenv) = LFHT_NodeNext(chain_node);

  if (!LFHT_IsHashLevel(chain_next)) {
    LFHT_ThreadMemRef(tenv) = chain_next;
    LFHT_UnsetThreadMemRefNext(tenv); 
    return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash,
	     chain_next, key, n_shifts, cn);
  }  
  // chain_next is a hash pointer
  if ((LFHT_STR_PTR *)chain_next == curr_hash) {
    if (cn == LFHT_MAX_NODES_PER_BUCKET) {
      LFHT_STR_PTR *new_hash;
      LFHT_STR_PTR *bucket;
      LFHT_AllocBuckets(new_hash, curr_hash, LFHT_STR, tenv);
      new_hash = 
	(LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);

      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), 
		       (LFHT_STR_PTR) curr_hash,
		       (LFHT_STR_PTR) new_hash)) {
	/* get head of the chain to be adjusted */
	LFHT_GetBucket(bucket, curr_hash, key, n_shifts, 
	  LFHT_STR); 
	/* adjust nodes in the chain */
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, *bucket, n_shifts);
	LFHT_ThreadMemRef(tenv) = (LFHT_STR_PTR) new_hash;
	/* chain bucket with next hash level */
        LFHT_SetBucket(bucket, new_hash, LFHT_STR);
	return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(new_hash, 
		 key, (n_shifts + 1));
      } else {
	LFHT_FreeBuckets(new_hash, tenv);
      }
    } else {
      LFHT_STR_PTR new_node;
      LFHT_NEW_NODE(new_node, key, (LFHT_STR_PTR) curr_hash, 
        tenv);
      LFHT_ThreadMemRefNext(tenv) = new_node;
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), 
		       (LFHT_STR_PTR) curr_hash, new_node)) {
#ifdef LFHT_DEBUG
	total_nodes++;
#endif /* LFHT_DEBUG */
	return new_node;
      }
      LFHT_FREE_NODE(new_node, tenv);
    }
    chain_next = LFHT_ThreadMemRef(tenv) = LFHT_NodeNext(chain_node);

    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash,
     	       chain_next, key, n_shifts, cn);
  }
  /* chain_next is refering a deeper hash level. 
     Thread must jump its hash level */

  LFHT_UnsetThreadMemRef(tenv);
  LFHT_UnsetThreadMemRefNext(tenv);

  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash != curr_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(jump_hash, key, (n_shifts + 1));
}


/*
static inline LFHT_STR_PTR LFHT_CHECK_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, 
							  LFHT_STR_PTR chain_node,  
							  LFHT_NODE_KEY_STR key, 
							  int n_shifts, 
							  int count_nodes  
							  LFHT_USES_ARGS) {

  if (LFHT_IsEqualKey(chain_node, key))
    return chain_node;
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  chain_next = LFHT_NodeNext(chain_node);

  if (!LFHT_IsHashLevel(chain_next))
    return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash, chain_next, 
					       key, n_shifts, cn);
  
  // chain_next is a hash pointer
  if ((LFHT_STR_PTR *)chain_next == curr_hash) {
    if (cn == LFHT_MAX_NODES_PER_BUCKET) {
      LFHT_STR_PTR *new_hash;
      LFHT_STR_PTR *bucket;
      LFHT_AllocBuckets(new_hash, curr_hash, LFHT_STR, tenv);
      new_hash = (LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);
      LFHT_GetBucket(bucket, new_hash, LFHT_NodeKey(chain_node), 
		     (n_shifts + 1), LFHT_STR);

      LFHT_SetBucket(bucket, chain_node, LFHT_STR);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), (LFHT_STR_PTR) curr_hash, 
		       (LFHT_STR_PTR) new_hash)) {
	LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, *bucket, chain_node, n_shifts);
        LFHT_SetBucket(bucket, new_hash, LFHT_STR);
	return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(new_hash, key, (n_shifts + 1));
      } else 
	LFHT_FreeBuckets(new_hash, tenv);
    } else {
      LFHT_STR_PTR new_node;
      LFHT_NEW_NODE(new_node, key, (LFHT_STR_PTR) curr_hash, tenv);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), (LFHT_STR_PTR) curr_hash, 
		       new_node)) {
#ifdef LFHT_DEBUG
	total_nodes++;
#endif // LFHT_DEBUG 
	return new_node;
      }
      LFHT_FREE_NODE(new_node, tenv);
    }
    chain_next = LFHT_NodeNext(chain_node);
    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_CHECK_INSERT_BUCKET_CHAIN(curr_hash, chain_next, 
						 key, n_shifts, cn);
  }
  // chain_next is refering a deeper hash level. Thread must jump its hash level
  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash != curr_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_CHECK_INSERT_BUCKET_ARRAY(jump_hash, key, (n_shifts + 1));
}
*/

static inline void 
  LFHT_ADJUST_CHAIN_NODES(LFHT_STR_PTR *new_hash, 
			  LFHT_STR_PTR chain_node, 
			  int n_shifts 
			  LFHT_USES_REGS) {


  if (chain_node == (LFHT_STR_PTR) new_hash)
    return;

  LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, LFHT_NodeNext(chain_node), n_shifts);

  LFHT_NodeNext(chain_node) = (LFHT_STR_PTR) new_hash;
  LFHT_ThreadMemRef(tenv) = chain_node;

  if (LFHT_IsDeletedKey(chain_node)) {    
    LFHT_InsertOnDeletePool(chain_node);
    return;
  }

  return LFHT_CALL_INSERT_BUCKET_ARRAY(new_hash, chain_node, (n_shifts + 1));
}


/*
static inline void LFHT_ADJUST_CHAIN_NODES(LFHT_STR_PTR *new_hash, 
					   LFHT_STR_PTR chain_node, 
					   LFHT_STR_PTR last_node, 
					   int n_shifts 
					   LFHT_USES_REGS) {
  if (chain_node == last_node)
    return;
  LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, LFHT_NodeNext(chain_node), 
			       last_node, n_shifts);
  return LFHT_CALL_INSERT_BUCKET_ARRAY(new_hash, chain_node, (n_shifts + 1));   
}

*/


static inline void LFHT_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash, 
					    LFHT_STR_PTR chain_node, 
					    int n_shifts 
					    LFHT_USES_REGS) {

  /* Don't forget that at this point LFHT_ThreadMemRef(tenv) = chain_node */

  LFHT_STR_PTR *bucket;
  LFHT_NodeNext(chain_node) = (LFHT_STR_PTR) curr_hash;
  LFHT_GetBucket(bucket, curr_hash, LFHT_NodeKey(chain_node),
    n_shifts, LFHT_STR);
  if (LFHT_IsEmptyBucket(*bucket, curr_hash, LFHT_STR))
    if (LFHT_BoolCAS(bucket, (LFHT_STR_PTR) curr_hash,
		     chain_node)) {
      return;
    }
  LFHT_STR_PTR bucket_next = LFHT_ThreadMemRefNext(tenv) = *bucket;
  if (LFHT_IsHashLevel(bucket_next))
    return LFHT_CALL_INSERT_BUCKET_ARRAY((LFHT_STR_PTR *)bucket_next, 
	     chain_node, (n_shifts + 1));
  return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, bucket_next, 
           chain_node, n_shifts, 0);
}

/*
static inline void LFHT_INSERT_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash, 
					    LFHT_STR_PTR chain_node, 
					    int n_shifts 
					    LFHT_USES_REGS) {
  LFHT_STR_PTR *bucket;
  LFHT_NodeNext(chain_node) = (LFHT_STR_PTR) curr_hash;
  LFHT_GetBucket(bucket, curr_hash, LFHT_NodeKey(chain_node), n_shifts, LFHT_STR);
  if (LFHT_IsEmptyBucket(*bucket, curr_hash, LFHT_STR))
    if (LFHT_BoolCAS(bucket, (LFHT_STR_PTR) curr_hash, chain_node)) {
      return;
    }
  LFHT_STR_PTR bucket_next = *bucket;
  if (LFHT_IsHashLevel(bucket_next))
    return LFHT_CALL_INSERT_BUCKET_ARRAY((LFHT_STR_PTR *)bucket_next, 
					 chain_node, (n_shifts + 1));
  return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, bucket_next, 
				       chain_node, n_shifts, 0);
}
*/


static inline void LFHT_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, 
					    LFHT_STR_PTR chain_node, 
					    LFHT_STR_PTR adjust_node, 
					    int n_shifts, 
					    int count_nodes 
					    LFHT_USES_REGS) {

  /* LFHT_ThreadMemRef(tenv) = adjust_node
     LFHT_ThreadMemRefNext(tenv) = chain_node */

  LFHT_NODE_KEY_STR key = LFHT_NodeKey(adjust_node);
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  chain_next = LFHT_ThreadMemRefNext(tenv) = LFHT_NodeNext(chain_node);
  if (!LFHT_IsHashLevel(chain_next))
    return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, chain_next, 
  	     adjust_node, n_shifts, cn);
  // chain_next is a hash pointer
  if ((LFHT_STR_PTR *)chain_next == curr_hash) {
    if (cn == LFHT_MAX_NODES_PER_BUCKET) {
      LFHT_STR_PTR *new_hash;
      LFHT_STR_PTR *bucket;
      LFHT_AllocBuckets(new_hash, curr_hash, LFHT_STR, tenv);
      new_hash = (LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), (LFHT_STR_PTR) curr_hash, 
		       (LFHT_STR_PTR) new_hash)) {
	LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);
        LFHT_ThreadMemRef(tenv) = *bucket;
	LFHT_UnsetThreadMemRefNext(tenv);
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, *bucket, n_shifts);
	LFHT_ThreadMemRef(tenv) = adjust_node;
        LFHT_SetBucket(bucket, new_hash, LFHT_STR);
	return LFHT_CALL_INSERT_BUCKET_ARRAY(new_hash,
                 adjust_node, (n_shifts + 1));
      } else
	LFHT_FreeBuckets(new_hash, tenv);
    } else {
      LFHT_NodeNext(adjust_node) = (LFHT_STR_PTR) curr_hash;
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), (LFHT_STR_PTR) curr_hash,
		       adjust_node)) {
	LFHT_UnsetThreadMemRefNext(tenv);
	return;
      }
    }
    chain_next = LFHT_ThreadMemRefNext(tenv) = LFHT_NodeNext(chain_node);
    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, chain_next, 
	       adjust_node, n_shifts, cn);
  }
  // chain_next is refering a deeper hash level. Thread must jump its hash level
  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash != curr_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_INSERT_BUCKET_ARRAY(jump_hash,
           adjust_node, (n_shifts + 1));
}


/*
static inline void LFHT_INSERT_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, 
					    LFHT_STR_PTR chain_node, 
					    LFHT_STR_PTR adjust_node, 
					    int n_shifts, 
					    int count_nodes 
					    LFHT_USES_REGS) {

  LFHT_NODE_KEY_STR key = LFHT_NodeKey(adjust_node);
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  chain_next = LFHT_NodeNext(chain_node);
  if (!LFHT_IsHashLevel(chain_next))
    return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, chain_next, 
  	     adjust_node, n_shifts, cn);
  // chain_next is a hash pointer
  if ((LFHT_STR_PTR *)chain_next == curr_hash) {
    if (cn == LFHT_MAX_NODES_PER_BUCKET) {
      LFHT_STR_PTR *new_hash;
      LFHT_STR_PTR *bucket;
      LFHT_AllocBuckets(new_hash, curr_hash, LFHT_STR, tenv);
      new_hash = (LFHT_STR_PTR *) LFHT_TagAsHashLevel(new_hash);
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), (LFHT_STR_PTR) curr_hash, 
		       (LFHT_STR_PTR) new_hash)) {
	LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);
	LFHT_CALL_ADJUST_CHAIN_NODES(new_hash, *bucket, n_shifts);
	LFHT_ThreadMemRef(tenv) = (LFHT_STR_PTR) new_hash;
        LFHT_SetBucket(bucket, new_hash, LFHT_STR);
	return LFHT_CALL_INSERT_BUCKET_ARRAY(new_hash,
                 adjust_node, (n_shifts + 1));
      } else
	LFHT_FreeBuckets(new_hash, tenv);
    } else {
      LFHT_NodeNext(adjust_node) = (LFHT_STR_PTR) curr_hash;
      if (LFHT_BoolCAS(&(LFHT_NodeNext(chain_node)), (LFHT_STR_PTR) curr_hash,
		       adjust_node))
	return;
    }
    chain_next = LFHT_NodeNext(chain_node);
    if (!LFHT_IsHashLevel(chain_next))
      return LFHT_CALL_INSERT_BUCKET_CHAIN(curr_hash, chain_next, 
	       adjust_node, n_shifts, cn);
  }
  // chain_next is refering a deeper hash level. Thread must jump its hash level
  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash != curr_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_INSERT_BUCKET_ARRAY(jump_hash,
           adjust_node, (n_shifts + 1));
}

*/


/* ------------------------------------------------------------------------------------*/
/*                     show state (prints the nodes inside the LFHT)                   */
/* ------------------------------------------------------------------------------------*/

static inline void LFHT_SHOW_STATE(void) {
  LFHT_STR_PTR first_node;
  LFHT_GetFirstNode(first_node);
  if (first_node == NULL) {
    printf("LFHT is empty \n");
    return;
  }  
  if (LFHT_IsHashLevel(first_node))
    return LFHT_SHOW_BUCKET_ARRAY((LFHT_STR_PTR *) first_node);
  return LFHT_SHOW_CHAIN(first_node, (LFHT_STR_PTR *)NULL);
}

static inline void LFHT_SHOW_CHAIN(LFHT_STR_PTR chain_node,
				   LFHT_STR_PTR *end_chain) {
  
  if ((LFHT_STR_PTR *) chain_node == end_chain)
    return;  
  LFHT_SHOW_CHAIN(LFHT_NodeNext(chain_node), end_chain);
  LFHT_SHOW_NODE(chain_node);
  return;
}

static inline void LFHT_SHOW_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash) {
  int i;
  LFHT_STR_PTR *bucket;
  bucket = (LFHT_STR_PTR *) LFHT_UntagHashLevel(curr_hash);
  for (i = 0; i < LFHT_BUCKET_ARRAY_SIZE; i++) {
    if ((LFHT_STR_PTR *) *bucket != curr_hash) {
      if (LFHT_IsHashLevel((*bucket))) {      
	LFHT_SHOW_BUCKET_ARRAY((LFHT_STR_PTR *) *bucket);
      } else
	LFHT_SHOW_CHAIN((LFHT_STR_PTR)*bucket, curr_hash);
    }
    bucket++;
  }
  return;
}


/* ------------------------------------------------------------------------------------*/
/*                     show statistics                                                 */
/* ------------------------------------------------------------------------------------*/

static inline void LFHT_SHOW_STATISTICS(void) {
  LFHT_STR_PTR first_node;
  LFHT_StatisticsResetCounters();
  LFHT_GetFirstNode(first_node);
  if (first_node == NULL) {
    printf("LFHT is empty \n");
    return;
  }  

  if (LFHT_IsHashLevel(first_node))
    return LFHT_SHOW_STATISTICS_BUCKET_ARRAY((LFHT_STR_PTR *) first_node);
  return LFHT_SHOW_STATISTICS_CHAIN(first_node, (LFHT_STR_PTR *) NULL);
  
}

static inline void LFHT_SHOW_STATISTICS_CHAIN(LFHT_STR_PTR chain_node,
					      LFHT_STR_PTR *end_chain) {
  
  if ((LFHT_STR_PTR *) chain_node == end_chain)
    return;  
  LFHT_SHOW_STATISTICS_CHAIN(LFHT_NodeNext(chain_node), end_chain);
  LFHT_SHOW_STATISTICS_NODE(chain_node);
  return;
}

static inline void LFHT_SHOW_STATISTICS_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash) {
  int i;
  LFHT_STR_PTR *bucket;
  bucket = (LFHT_STR_PTR *) LFHT_UntagHashLevel(curr_hash);
  for (i = 0; i < LFHT_BUCKET_ARRAY_SIZE; i++) {
    if ((LFHT_STR_PTR *) *bucket != curr_hash) {
      if (LFHT_IsHashLevel((*bucket))) {      
	LFHT_SHOW_STATISTICS_BUCKET_ARRAY((LFHT_STR_PTR *) *bucket);
      } else
	LFHT_SHOW_STATISTICS_CHAIN((LFHT_STR_PTR) *bucket, curr_hash);
    }
    bucket++;
  }
  return;
}


/* -------------------------------------------------------------------*/
/*    abolish all keys (removes all data structures inside the LFHT)  */
/*        (MAKE SURE THAT ONLY THREAD 0 IS WORKING IN THE LFHT)       */
/* -------------------------------------------------------------------*/

static inline void LFHT_ABOLISH_ALL_KEYS(void) {
  LFHT_STR_PTR first_node;
  LFHT_GetFirstNode(first_node);

  /* For now i just don't care about the deleted nodes pool */
  /* CHECK THIS LATER */
  LFHT_DeletePool(LFHT_ROOT_ENV) = NULL; 

  /* Making sure that thread 0 does not have any data structure in its buffers */
  LFHT_ThreadEnvPtr tenv = &(LFHT_ThreadEnv(LFHT_ROOT_ENV, 0));
  LFHT_ThreadUnusedNode(tenv) = LFHT_ThreadUnusedBucketArray(tenv) = NULL;
  /* Ready to abolish LFHT */  

  if (first_node == NULL) {
    printf("LFHT is empty \n");
    return;
  }  
  if (LFHT_IsHashLevel(first_node))
    LFHT_ABOLISH_BUCKET_ARRAY((LFHT_STR_PTR *) first_node);
  else
    LFHT_ABOLISH_CHAIN(first_node, (LFHT_STR_PTR *)NULL);
  LFHT_FirstNode = NULL;
  return;
}

static inline void LFHT_ABOLISH_CHAIN(LFHT_STR_PTR chain_node,
				      LFHT_STR_PTR * end_chain) {
  if ((LFHT_STR_PTR *) chain_node == end_chain)
    return;  
  LFHT_ABOLISH_CHAIN(LFHT_NodeNext(chain_node), end_chain);
  FREE_DIC_ENTRY(chain_node);
  return;
}

static inline void LFHT_ABOLISH_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash) {
  int i;
  LFHT_STR_PTR *bucket;
  bucket = (LFHT_STR_PTR *) LFHT_UntagHashLevel(curr_hash);
  for (i = 0; i < LFHT_BUCKET_ARRAY_SIZE ; i++) {
    if ((LFHT_STR_PTR *) *bucket != curr_hash) { 
      if (LFHT_IsHashLevel((*bucket))) 
	LFHT_ABOLISH_BUCKET_ARRAY((LFHT_STR_PTR *) *bucket);
      else
	LFHT_ABOLISH_CHAIN((LFHT_STR_PTR)*bucket, curr_hash);
    }
    bucket++;
  }
  LFHT_DeallocateBucketArray(curr_hash);
  return;
}

/* -----------------------------------------------------*/
/*        deletes a key from the LFHT                   */
/* -----------------------------------------------------*/

static inline LFHT_STR_PTR 
  LFHT_CHECK_DELETE_KEY(LFHT_NODE_KEY_STR key 
			LFHT_USES_ARGS) {
  
  LFTH_ShiftDeleteBits(key);
  LFHT_GetFirstNode(LFHT_ThreadMemRef(tenv));

  if (LFHT_ThreadMemRef(tenv) == NULL)
    return NULL;
  
  LFHT_STR_PTR node;
  if (LFHT_IsHashLevel(LFHT_ThreadMemRef(tenv)))
    node = LFHT_CALL_CHECK_DELETE_BUCKET_ARRAY(
            (LFHT_STR_PTR *) LFHT_ThreadMemRef(tenv),  key, 0);
  else
    node = LFHT_CALL_CHECK_DELETE_FIRST_CHAIN(
           LFHT_ThreadMemRef(tenv), key, 0);

  if (++LFHT_ThreadNrOfOps(tenv) == LFHT_FREE_TO_DELETE_POOL) {
    LFHT_ThreadNrOfOps(tenv) = 0;
    LFHT_FreeToDeletePool();
  }

  return node;
}

static inline LFHT_STR_PTR 
  LFHT_CHECK_DELETE_FIRST_CHAIN(LFHT_STR_PTR chain_node,
				LFHT_NODE_KEY_STR key, 
				int count_nodes 
				LFHT_USES_ARGS) {
  /* Don't forget that:
     at this point LFHT_ThreadMemRef(tenv) = chain_node 
     tenv is hidden in LFHT_USES_ARGS */

  if (LFHT_IsEqualKey(chain_node, key)) {
    LFHT_TagAsDeletedKey(chain_node);
    return chain_node;
  }
  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  /* thread is still working in chain_node */
  chain_next = 
    LFHT_ThreadMemRefNext(tenv) = LFHT_NodeNext(chain_node);

  if (chain_next && !LFHT_IsHashLevel(chain_next)) {
    LFHT_ThreadMemRef(tenv) = chain_next;
    LFHT_UnsetThreadMemRefNext(tenv); // this might be useless !!
    return LFHT_CALL_CHECK_DELETE_FIRST_CHAIN(chain_next, key, cn); 
  }
  /* chain_next refering the end of the chain or 
     is a hash level pointer */
  if (chain_next == NULL)
    return NULL;  
  /* chain_next is refering a deeper hash level. 
     Thread must jump a previous hash level */
  LFHT_UnsetThreadMemRef(tenv);
  LFHT_UnsetThreadMemRefNext(tenv);

  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_CHECK_DELETE_BUCKET_ARRAY(jump_hash, key, 0);
}

static inline LFHT_STR_PTR 
  LFHT_CHECK_DELETE_BUCKET_ARRAY(LFHT_STR_PTR *curr_hash,
				 LFHT_NODE_KEY_STR key, 
				 int n_shifts 
				 LFHT_USES_ARGS) {

  /* Don't forget that at this point 
     LFHT_ThreadMemRef(tenv) = curr_hash */
  LFHT_STR_PTR *bucket;
  LFHT_GetBucket(bucket, curr_hash, key, n_shifts, LFHT_STR);
  
  if (LFHT_IsEmptyBucket(*bucket, curr_hash, LFHT_STR)) 
    return NULL;
  
  LFHT_STR_PTR bucket_next =
    LFHT_ThreadMemRef(tenv) = *bucket;

  if (LFHT_IsHashLevel(bucket_next))
    return LFHT_CALL_CHECK_DELETE_BUCKET_ARRAY(
             (LFHT_STR_PTR *)bucket_next, key, (n_shifts + 1));

  return LFHT_CALL_CHECK_DELETE_BUCKET_CHAIN(curr_hash, 
           bucket_next, key, n_shifts, 0);
}

static inline LFHT_STR_PTR 
  LFHT_CHECK_DELETE_BUCKET_CHAIN(LFHT_STR_PTR *curr_hash, 
				 LFHT_STR_PTR chain_node,  
				 LFHT_NODE_KEY_STR key, 
				 int n_shifts, 
				 int count_nodes  
				 LFHT_USES_ARGS) {

  /* Don't forget that at this point 
     LFHT_ThreadMemRef(tenv) = chain_node */

  if (LFHT_IsEqualKey(chain_node, key)) {
    LFHT_TagAsDeletedKey(chain_node);
    return chain_node;
  }

  int cn = count_nodes + 1;
  LFHT_STR_PTR chain_next;
  chain_next = 
    LFHT_ThreadMemRefNext(tenv) = LFHT_NodeNext(chain_node);

  if (!LFHT_IsHashLevel(chain_next)) {
    LFHT_ThreadMemRef(tenv) = chain_next;
    LFHT_UnsetThreadMemRefNext(tenv); 
    return LFHT_CALL_CHECK_DELETE_BUCKET_CHAIN(curr_hash,
	     chain_next, key, n_shifts, cn);
  }  
  // chain_next is a hash pointer
  if ((LFHT_STR_PTR *)chain_next == curr_hash) 
    return NULL;
    
  /* chain_next is refering a deeper hash level. 
     Thread must jump its hash level */

  LFHT_UnsetThreadMemRef(tenv);
  LFHT_UnsetThreadMemRefNext(tenv);

  LFHT_STR_PTR *jump_hash, *prev_hash;
  jump_hash = (LFHT_STR_PTR *) chain_next;
  LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  while (prev_hash != curr_hash) {
    jump_hash = prev_hash;
    LFHT_GetPreviousHashLevel(prev_hash, jump_hash, LFHT_STR);
  }
  return LFHT_CALL_CHECK_DELETE_BUCKET_ARRAY(jump_hash, key, (n_shifts + 1));
}


/* -------------------------------------------------------*/
/*                 Free To Delete Pool                    */
/* -------------------------------------------------------*/

static inline
  void LFHT_FreeToDeletePool(void) {
    LFHT_ToDeletePtr to_delete_node;
    LFHT_ToDeletePtr unfree_head = NULL;
    LFHT_ToDeletePtr unfree_tail = NULL;
      
    if ((to_delete_node = LFHT_ValCAS((&(LFHT_DeletePool(LFHT_ROOT_ENV))),
			    LFHT_DeletePool(LFHT_ROOT_ENV), NULL)) == NULL)
      /* ToDeletePool is empty */
      return;

    while (to_delete_node) {
      void *chain_node = (void *) LFHT_ToDeleteNode(to_delete_node);
      int i;
      /* optimize this search ...please  */
      for (i = 0; i < LFHT_MAX_THREADS; i++)
	if (LFHT_ThreadEnv(LFHT_ROOT_ENV, i).mem_ref == chain_node ||
	    LFHT_ThreadEnv(LFHT_ROOT_ENV, i).mem_ref_next == chain_node)
	  break;

      LFHT_ToDeletePtr free_to_delete_node = to_delete_node;
      to_delete_node = LFHT_ToDeleteNext(to_delete_node);

      if (i == LFHT_MAX_THREADS) {
	LFHT_SHOW_TO_DELETE_NODE((LFHT_STR_PTR) chain_node);
	free(chain_node);
	free(free_to_delete_node);
      } else {
	LFHT_ToDeleteNext(free_to_delete_node) = unfree_head;
	unfree_head = free_to_delete_node;
	if (unfree_tail == NULL)
	  unfree_tail = unfree_head;
      }
    }
    if (unfree_tail != NULL) {
      /* join again to the toDeletePool */
      do { 
	LFHT_ToDeleteNext(unfree_tail) = LFHT_DeletePool(LFHT_ROOT_ENV);
      } while(!LFHT_BoolCAS((&(LFHT_DeletePool(LFHT_ROOT_ENV))),	
			    LFHT_ToDeleteNext(unfree_tail), unfree_head));
    }
    return;
}


/* ------------------------------------------------------------------------------------*/
/*                                 undefine macros                                     */
/* ------------------------------------------------------------------------------------*/

#undef LFHT_STR
#ifndef LFHT_DEBUG
#undef LFHT_STR_PTR
#undef LFHT_NodeKey
#undef LFHT_NodeNext
#endif /* LFHT_DEBUG */
#undef LFHT_USES_ARGS
#undef LFHT_PASS_ARGS
#undef LFHT_ROOT_ADDR
#undef LFHT_GetFirstNode
#undef LFHT_FirstNode
#undef LFHT_ALLOC_NODE
#undef LFHT_DEALLOC_NODE
#undef LFHT_CHECK_INSERT_KEY
#undef LFHT_CHECK_DELETE_KEY
#undef LFHT_CHECK_INSERT_FIRST_CHAIN
#undef LFHT_CHECK_DELETE_FIRST_CHAIN
#undef LFHT_CHECK_INSERT_BUCKET_ARRAY
#undef LFHT_CHECK_DELETE_BUCKET_ARRAY
#undef LFHT_CHECK_INSERT_BUCKET_CHAIN
#undef LFHT_CHECK_DELETE_BUCKET_CHAIN
#undef LFHT_ADJUST_CHAIN_NODES
#undef LFHT_INSERT_BUCKET_ARRAY
#undef LFHT_INSERT_BUCKET_CHAIN
#undef LFHT_SHOW_STATE
#undef LFHT_SHOW_CHAIN
#undef LFHT_SHOW_BUCKET_ARRAY
#undef LFHT_ABOLISH_ALL_KEYS
#undef LFHT_ABOLISH_CHAIN
#undef LFHT_ABOLISH_BUCKET_ARRAY
