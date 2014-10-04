#include "bench.h"

int main() {

  int key, value = 2;  
  for (key = 1 ; key <=10 ; key++)
    dic_check_insert_key(key, value);

  /* Don't use the following functions 
     in a concurrent environment */
  dic_show_state();
  dic_abolish_all_keys();
  dic_show_state();

  return 0;
}






