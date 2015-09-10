#include<stdio.h>
#include<stdlib.h>

/* Define your structure */
typedef struct dic {
  long key;           /* LFHT Mandatory Field */
  long value;         /* Optional  Field */
  struct dic *next;   /* LFHT Mandatory Field */
} *dic_ptr;

#define Dic_key(X)    ((X)->key)
#define Dic_val(X)    ((X)->value)
#define Dic_next(X)   ((X)->next)

/* Define how a key is inserted */

#define NEW_DIC_ENTRY(PTR, KEY, VAL, NEXT) {			   \
  if ((PTR = (struct dic *) malloc(sizeof(struct dic))) == NULL)   \
    perror("ALLOC_DIC_ENTRY: malloc error") ;			   \
  Dic_key(PTR) = KEY;						   \
  Dic_val(PTR) = VAL;						   \
  Dic_next(PTR) = NEXT;						   \
}

/* Define how a key is released */
#define FREE_DIC_ENTRY(PTR)         free(PTR)
/* Define how a key is shown */
#define SHOW_DIC_ENTRY(NODE)        printf("(%ld, %ld)\n", Dic_key(NODE), Dic_val(NODE))

/* Define the external point from where the LFHT is called */

struct benchRoot {dic_ptr dic;};
struct benchRoot Root;

/* Include the files in the project */

#define INCLUDE_DIC_LOCK_FREE_HASH_TRIE  1


#define LFHT_DEBUG 1
#ifdef LFHT_DEBUG
int total_nodes = 0;
#endif /* LFHT_DEBUG */

#include "LFHT_tries.i"
