#ifndef _LFHT_PARAMETERS_H
#define _LFHT_PARAMETERS_H

/*******************************************************************************
 *                            LFHT thread parameters                           *
 *******************************************************************************/

/* Define the maximum number of simultaneous threads that LFHT will support */
#define LFHT_MAX_THREADS 32
/* Define the frequency in which a thread tries to free the delete pool */
#define LFHT_FREE_TO_DELETE_POOL 20
/*******************************************************************************
 *                            LFHT data parameters                             *
 *******************************************************************************/
#define LFHT_BIT_SHIFT                    3
#define LFHT_BUCKET_ARRAY_SIZE            (1 << LFHT_BIT_SHIFT)
#define LFHT_MAX_NODES_PER_BUCKET         4
#define LFHT_CELL                         long

/*******************************************************************************
 *                            LFHT internals                                   *
 *******************************************************************************/

/* LFHT statistics are calculated at the end of the execution of all
   threads.  When the main thread calls the show_statistics function
   the LFHT data structures are traversed and statistics are taken. No
   synchronization envolved. */

//#define LFHT_STATISTICS       1 --> NOT USED

/* LFHT statistics are taken during the execution of threads. 
   Threads have local counters and at the end of their execution they  
   pass their values to the global counters. Synchronization occurs then threads 
   end their execution. OBS: The main thread still has to call the show_statistics
   function to display the data. */

//#define LFHT_THREAD_STATISTICS       1  --> NOT IMPLEMENTED YET


#endif /* _LFHT_PARAMETERS_H */
