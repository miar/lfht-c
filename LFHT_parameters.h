#ifndef _LFHT_PARAMETERS_H
#define _LFHT_PARAMETERS_H

/*******************************************************************************
 *                            LFHT thread parameters                           *
 *******************************************************************************/

/* Define the maximum number of simultaneous threads that your system supports */
#define LFHT_MAX_THREADS 32

/*******************************************************************************
 *                            LFHT data parameters                             *
 *******************************************************************************/
#define LFHT_BIT_SHIFT                    3
#define LFHT_BUCKET_ARRAY_SIZE            (1 << LFHT_BIT_SHIFT)
#define LFHT_MAX_NODES_PER_BUCKET         4
#define LFHT_CELL                         long

#endif /* _LFHT_PARAMETERS_H */
