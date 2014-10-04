#include "bench.h"

int main() {

  int key, value = 2;  
  for (key = 1 ; key <=10 ; key++)
    dic_check_insert_key(key, value);
  dic_show_state();
  dic_abolish_all_keys();
  return 0;
}






