
 %module lfht
 %{
   #include "lfht.h"

   %}

extern dic_ptr dic_check_insert_key(long, long, LFHT_ThreadEnvPtr);

extern dic_ptr dic_check_delete_key(long, long, LFHT_ThreadEnvPtr);

extern void dic_show_state(char *);

extern void dic_show_statistics(char *);

extern void dic_abolish_all_keys(void);

extern void dic_show_delete_pool(char *);

extern void dic_create_init_env(void);

extern LFHT_ThreadEnvPtr dic_create_init_thread_env(int);

extern void dic_kill_thread_env(int);
