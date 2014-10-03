#include<stdio.h>
#include<stdlib.h>

typedef struct dic {
  long key;            /* Mandatory Field */
  long value;          /* Optional  Field */
  struct dic *next;   /* Mandatory Field */
} *dic_ptr;

#define Dic_key(X)    ((X)->key)
#define Dic_val(X)    ((X)->value)
#define Dic_next(X)   ((X)->next)

#define NEW_DIC_ENTRY(PTR, KEY, VAL, NEXT) {			   \
  if ((PTR = (struct dic *) malloc(sizeof(struct dic))) == NULL)   \
    perror("ALLOC_DIC_ENTRY: malloc error") ;			   \
  Dic_key(PTR) = KEY;						   \
  Dic_val(PTR) = VAL;						   \
  Dic_next(PTR) = NEXT;						   \
}

#define FREE_DIC_ENTRY(PTR)         free(PTR)


struct benchRoot {dic_ptr Root;};
struct benchRoot Root;


#define INCLUDE_DIC_LOCK_FREE_HASH_TRIE  1

#include "lockFreeHash.tries.i"


