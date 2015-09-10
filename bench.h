#include<stdio.h>
#include<stdlib.h>
#include "LFHT_tries.h"

/* Define your node within the LFHT data structure */
typedef struct dic {
  long key;           /* LFHT Mandatory Field */
  long value;         /* Optional  Field */
  struct dic *next;   /* LFHT Mandatory Field */
} *dic_ptr;

#define Dic_key(X)    ((X)->key)
#define Dic_val(X)    ((X)->value)
#define Dic_next(X)   ((X)->next)

/* Define how your data structure is inserted */

#define NEW_DIC_ENTRY(PTR, KEY, VAL, NEXT) {			   \
  if ((PTR = (struct dic *) malloc(sizeof(struct dic))) == NULL)   \
    perror("ALLOC_DIC_ENTRY: malloc error") ;			   \
  Dic_key(PTR) = KEY;						   \
  Dic_val(PTR) = VAL;						   \
  Dic_next(PTR) = NEXT;						   \
}

/* Define how your data structure is released */
#define FREE_DIC_ENTRY(PTR)             free(PTR)

/* Define how your data structure is shown */
#define SHOW_DIC_ENTRY(PTR, KEY)        printf("(%ld, %ld)\n", KEY, Dic_val(PTR))

/* Define where you want to hook the LFHT data structure */
struct benchRoot {
  dic_ptr dic; /* Memory address where you want to hook the LFHT data structures */
  LFHT_EnvPtr dic_env; /* Memory address where you want to hook the LFHT environment */
};

struct benchRoot Root;

/* Include the files in the project */

#define INCLUDE_DIC_LOCK_FREE_HASH_TRIE  1

#define LFHT_DEBUG 1
#ifdef LFHT_DEBUG
int total_nodes = 0;
#endif /* LFHT_DEBUG */

#include "LFHT_tries.i"
