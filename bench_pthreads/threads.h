#ifndef __THREADS_H
#define __THREADS_H

//#define SET_THREAD_AFFINITY  1
/*#ifdef SET_THREAD_AFFINITY
#define CPU_LAUNCHER_OFFSET  6
#define _GNU_SOURCE
#endif */

#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "migshash.h"

/*****************************************************************************/
/*                            BENCH CONFIGURATOR                             */
/*****************************************************************************/

#define SINGLE_THREAD_EXECUTION   1
//#define CPUTIME_ON_THREAD_RUSAGE 1
#define CPUTIME_ON_THREAD_DAYTIME 1
//#define FLUSH_HASH_NODES 1 
//#define FLUSH_HASH_STATISTICS 1
//#define CREATE_NEW_DATA_SET_RANDOM 1
#define CREATE_NEW_DATA_SET_INCREMENTAL 1

#define DIVIDE_WORK_BETWEEN_THREADS   1 /* to disable check first IS_EQUAL_TO_NODE_ENTRY */

#define NUM_THREADS 32   /* NUM_THREADS > 1 */
#define RANDOM_TERM_RANGE  500000000 /* n is random number in range of 0 - 9 */

//#define DATASET_SIZE (NUM_THREADS * 50000)
//#define DATASET_SIZE (64 * 50000)


//#define DATASET_SIZE (NUM_THREADS * 50)
#define DATASET_SIZE   10000000


/*****************************************************************************/

#ifndef CACHE_LINE_OFFSET
#define CACHE_LINE_OFFSET 8 /* cache_line_size = 64 bytes (4 bytes(pointer) * 8 array pos) */
#endif

FILE *FP;
const char fdata_set[] = "output/data_set";
const char fcorrect_hash[] = "output/correct_hash";
const char fresult_hash[] = "output/result_hash";
long dataSet[DATASET_SIZE];
static volatile int wait_;

#ifdef FLUSH_HASH_STATISTICS
int total_nodes,  
  total_buckets, 
  total_empties,
  total_max_nodes, 
  total_min_nodes;
#endif /* FLUSH_HASH_STATISTICS */


typedef struct thread_work {
  int wid;
  long startI; 
  long endI;
#if defined(CPUTIME_ON_THREAD_RUSAGE)
  int execUTime;
  int execSTime;
#elif defined (CPUTIME_ON_THREAD_DAYTIME)
  struct timeval execStartTime;
  struct timeval execEndTime;  
#endif  
} *thread_work_ptr; 

struct thread_work tw_single;

#define SYNC_ADD(PTR, VALUE)   __sync_add_and_fetch(PTR, VALUE)
static inline void free_bucket_array(ans_node_ptr *, long);
static inline void free_bucket_chain(ans_node_ptr, int);

static inline void free_bucket_chain(ans_node_ptr chain_node, int count_nodes) {
  if (IS_HASH(chain_node)) {
#ifdef FLUSH_HASH_STATISTICS
    if (count_nodes > total_max_nodes)
      total_max_nodes = count_nodes;
    if (count_nodes < total_min_nodes)
      total_min_nodes = count_nodes;
#endif // FLUSH_HASH_STATISTICS 
    return;
  }
#ifdef FLUSH_HASH_NODES
  fprintf(FP, " %ld ", TrNode_entry(chain_node));
#endif
  free_bucket_chain(TrNode_next(chain_node), count_nodes + 1);
#ifdef FLUSH_HASH_STATISTICS
  total_nodes++;
#endif // FLUSH_HASH_STATISTICS 
  FREE_ANSWER_TRIE_NODE(chain_node);
  return;
}

static inline void free_bucket_array(ans_node_ptr *curr_hash, long level) {
  ans_node_ptr *bucket;
  int bucket_entry = 0;
  do {
#ifdef FLUSH_HASH_NODES
    fprintf(FP, "\n bkt entry -> %d (level = %ld) \n", bucket_entry, level);
#endif
#ifdef FLUSH_HASH_STATISTICS
    total_buckets++;
#endif // FLUSH_HASH_STATISTICS 
    bucket = (ans_node_ptr *)UNTAG(curr_hash) + bucket_entry;
    if (!IS_EMPTY_BUCKET(*bucket, curr_hash)) {
      if (IS_HASH(*bucket))
	free_bucket_array((ans_node_ptr *)*bucket, level + 1);
      else 
	free_bucket_chain(*bucket, 0);  
#ifdef FLUSH_HASH_NODES	
      fprintf(FP, "\n");	
#endif	
    } 
#ifdef FLUSH_HASH_STATISTICS
    else
      total_empties++;
#endif // FLUSH_HASH_STATISTICS 
  } while (++bucket_entry < BASE_HASH_BUCKETS);
  FREE_TRIE_HASH_BUCKETS(curr_hash);
  return;
}

/* gcc bug */
#ifndef RUSAGE_THREAD
#define RUSAGE_THREAD  1
#endif

#endif /* __THREADS_H */
