#include "bench.h"

int main() {

  int key = 1, value = 2;  
  
  dic_check_insert_key(key, value);
  dic_check_insert_key(2, value);
  dic_check_insert_key(3, value);
  dic_check_insert_key(4, value);
  dic_check_insert_key(5, value);

  /*  key = 3; value = 4;
  check_insert (root, key, value);
  print_values (root); */
  return 0;
}












