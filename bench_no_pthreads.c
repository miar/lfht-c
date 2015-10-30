#include "bench.h"

//#define NKEYS 10000

#define NKEYS 12



int main() {
  int key, value = 2;

  dic_create_init_env();
  
  int tid = 0;
  /* get thread's environment */
  LFHT_ThreadEnvPtr tenv = dic_create_init_thread_env(tid);

  printf("------ INSERT / DELETE -------\n");
  for (key = 1; key <= NKEYS; key++) {
    printf("key - %d\n", key);
    dic_check_insert_key(key, DIC_VALUE, tenv);
    if (key % 2 == 0)
      dic_check_delete_key(key, DIC_VALUE, tenv);
  }
  printf("1-total_nodes = %d\n", total_nodes);

  total_nodes = 0;

  dic_show_state();
  dic_show_delete_pool();
  
  dic_show_statistics();
  dic_kill_env();

  //  dic_abolish_all_keys();
  //dic_show_state();

  return 0;
}

