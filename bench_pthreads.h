#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "bench.h"

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