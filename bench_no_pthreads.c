#include "bench.h"

//#define NKEYS 10000

#define NKEYS 100

extern int total_nodes; 

int main() {
  int key, value = 2;

  LFHT_InitEnv(Root.dic_env);
  int tid = 0;
  /* get thread's environment */
  LFHT_ThreadEnvPtr tenv = LFHT_InitThreadEnv(Root.dic_env, tid);

  printf("------ INSERT / DELETE -------\n");
  for (key = 1; key <= NKEYS; key++) {
    printf("key - %d\n", key);
    dic_check_insert_key(key, value, tenv);
    if (key % 2 == 0)
      dic_check_delete_key(key, value, tenv);
  }
  printf("1-total_nodes = %d\n", total_nodes);

  total_nodes = 0;
  //  dic_show_state();

  dic_show_state();

  printf("------ DELETE POOL  -------\n");
  LFHT_ShowDeletePool();

  dic_abolish_all_keys();
  dic_show_state();

  /*
  for (key = 1 ; key <= NKEYS ; key++)
    dic_check_insert_key(key, value, tenv);

  dic_show_state();
  printf("2-total_nodes = %d\n", total_nodes);
  */
  //dic_abolish_all_keys();

  /* Don't use the following functions 
     in a concurrent environment */
  
//  dic_show_state();
  //dic_abolish_all_keys();
  //dic_show_state();

  return 0;
}

