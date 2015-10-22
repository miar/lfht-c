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

#define LFHT_STATISTICS       1






#endif /* _LFHT_PARAMETERS_H */
