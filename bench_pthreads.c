#include "bench_pthreads.h"

void *thread_run(void *ptr) { 
  struct thread_work *tw;
  tw = (struct thread_work *) ptr;
  long i;

#ifdef THREAD_FLUSH_EXECUTION
  char thr_out_file[25];
  if (tw == &tw_single)
    sprintf(thr_out_file, "output/thread_single");
  else
    sprintf(thr_out_file, "output/thread_%d", tw->wid);

  FILE *thr_out;
  thr_out = fopen(thr_out_file, "w");
#endif /* THREAD_FLUSH_EXECUTION */

#ifdef SET_THREAD_AFFINITY
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET((tw->wid) * CPU_LAUNCHER_OFFSET, &cpuset); 
  pthread_setaffinity_np(pthread_self(),sizeof(cpuset),&cpuset);
#endif

  /* wait for all threads to be ready */
  (void)SYNC_ADD(&wait_, -1);
  do {} while (wait_); 
   
#if defined(CPUTIME_ON_THREAD_RUSAGE)
  struct rusage tv1,tv2; 
  getrusage(RUSAGE_THREAD, &tv1);
#elif defined (CPUTIME_ON_THREAD_DAYTIME)    
  gettimeofday(&(tw->execStartTime), NULL); 
#endif
  int s = tw->startI, e = tw->endI;
  LFHT_ThreadEnvPtr tenv = tw->tenv;
  for (i = s; i < e; i++) {  
    dic_check_insert_key(dataSet[i], DIC_VALUE, tenv);
#ifdef THREAD_FLUSH_EXECUTION
    fprintf(thr_out, "Insert -> %ld\n", dataSet[i]);
#endif /* THREAD_FLUSH_EXECUTION */
    if (dataSet[i] % 2 == 0) {
      dic_check_delete_key(dataSet[i], DIC_VALUE, tenv);
#ifdef THREAD_FLUSH_EXECUTION
      fprintf(thr_out, "Delete -> %ld\n", dataSet[i]);
#endif /* THREAD_FLUSH_EXECUTION */
    }
  }

#if defined(CPUTIME_ON_THREAD_RUSAGE)
  getrusage(RUSAGE_THREAD, &tv2);
  tw->execUTime = (int)(((tv2.ru_utime.tv_sec  - tv1.ru_utime.tv_sec)  * 1000000 +
          (tv2.ru_utime.tv_usec - tv1.ru_utime.tv_usec)) / 1000); 
  tw->execSTime = (int)(((tv2.ru_stime.tv_sec  - tv1.ru_stime.tv_sec)  * 1000000 +
          (tv2.ru_stime.tv_usec - tv1.ru_stime.tv_usec)) / 1000);  
#elif defined (CPUTIME_ON_THREAD_DAYTIME)
  gettimeofday(&(tw->execEndTime), NULL); 
#endif
#ifdef THREAD_FLUSH_EXECUTION
  fclose(thr_out);
#endif /* THREAD_FLUSH_EXECUTION */

  pthread_exit(NULL);
}


void create_bench_and_solution(void) {
  long i;

#ifdef CREATE_NEW_DATA_SET_RANDOM
  /* create data set file */
  FP = fopen(fdata_set, "w");
  /* initialize the random seed */
  srand(time(NULL)); 
  for (i = 0; i < DATASET_SIZE; i++) { 
    dataSet[i] = rand() % RANDOM_TERM_RANGE;
    fprintf(FP, " %ld ", dataSet[i]);
  }
#elif defined(CREATE_NEW_DATA_SET_INCREMENTAL)
  /* create data set file */
  FP = fopen(fdata_set, "w");  
  for (i = 0; i < DATASET_SIZE; i++) {
    dataSet[i] = i;
    fprintf(FP, " %ld ", dataSet[i]);
  }    
#else /* LOAD PREVIOUS DATASET */  
  FP = fopen(fdata_set, "r");
  if (FP == NULL){
    printf("data_set does not exists (CREATE_NEW_DATA_SET) \n");
    exit(1);
  }
  for (i = 0 ; i < DATASET_SIZE ; i++)
    fscanf(FP,"%ld", &(dataSet[i]));
#endif /*CREATE_NEW_DATA_SET */
  fclose(FP);  

#ifdef SINGLE_THREAD_EXECUTION  
  dic_create_init_env();
  LFHT_ThreadEnvPtr tenv = dic_create_init_thread_env(0);

  wait_ = 1;
#if defined(CPUTIME_ON_THREAD_RUSAGE) || defined(CPUTIME_ON_THREAD_DAYTIME)  
  pthread_t threads;  
  tw_single.wid = 0;
  tw_single.tenv = tenv;
  tw_single.startI = 0;
  tw_single.endI = DATASET_SIZE;
  
  if (pthread_create(&threads, NULL, thread_run, (void *)(&tw_single))) {
    printf("ERROR: pthread_create() \n" );
    exit(-1);
  }
  void * status;	
  if (pthread_join(threads, &status)) {	  
    printf("ERROR: pthread_join() \n");
    exit(-1);
  } 

#if defined(CPUTIME_ON_THREAD_RUSAGE)
  printf(" Cputime RUSAGE THREAD (milliseconds): 1_thread = %d ", 
         tw_single.execUTime + tw_single.execSTime); 
#else /* !CPUTIME_ON_THREAD_RUSAGE */
  printf(" Cputime DAYTIME THREAD (milliseconds): 1_thread = %d ",
         (int) (1000000*(tw_single.execEndTime.tv_sec - tw_single.execStartTime.tv_sec) + 
                tw_single.execEndTime.tv_usec - tw_single.execStartTime.tv_usec) / 1000); 
#endif /* CPUTIME_ON_THREAD_RUSAGE */
#else  /* !CPUTIME_ON_THREAD_RUSAGE && !CPUTIME_ON_THREAD_DAYTIME */
  struct timeval tv1, tv2;  
  gettimeofday(&tv1, NULL);

  for (i = 0; i < DATASET_SIZE; i++)
    dic_check_insert_key(dataSet[i], DIC_VALUE, tenv);

  gettimeofday(&tv2, NULL);
  int ms = (int) (1000000*(tv2.tv_sec - tv1.tv_sec) + tv2.tv_usec - tv1.tv_usec) / 1000;
  printf(" Cputime DAYTIME MAIN (milliseconds): 1_thread = %d ", ms);     
#endif /* CPUTIME_ON_THREAD_RUSAGE || CPUTIME_ON_THREAD_DAYTIME */

  total_nodes = 0;
  
  dic_show_state(fcorrect_hash);
//  dic_show_delete_pool(fcorrect_hash);
//  dic_show_statistics(fcorrect_hash); // dic_show_statistics("stdout")
  dic_kill_env();

#endif /* SINGLE_THREAD_EXECUTION */

  return;
}

extern int total_nodes; 

int main(void) {
  pthread_t threads[NUM_THREADS];  
  int wid = 0;
  create_bench_and_solution();
  
  // run test
  dic_create_init_env();
  //new_answer_trie_hash_node(); 
  FP = fopen(fdata_set, "r");
  struct thread_work tw[NUM_THREADS];  
#ifdef DIVIDE_WORK_BETWEEN_THREADS
  int offset = (int)(DATASET_SIZE / NUM_THREADS);
  int startI = 0;
  do { 
    tw[wid].wid = wid;
    tw[wid].tenv = dic_create_init_thread_env(wid);
    tw[wid].startI = startI;
    tw[wid].endI = startI + offset;	
    startI = tw[wid].endI;
  } while(++wid < NUM_THREADS);    
#else
  do {
    tw[wid].wid = wid;
    tw[wid].tenv = dic_create_init_thread_env(wid);
    tw[wid].startI = 0;
    tw[wid].endI = DATASET_SIZE; 
  } while(++wid < NUM_THREADS);
#endif  
  // check time
  wait_ = NUM_THREADS;

#if !defined(CPUTIME_ON_THREAD_RUSAGE) && !defined(CPUTIME_ON_THREAD_DAYTIME)
  struct timeval tv1, tv2;  
  gettimeofday(&tv1, NULL); 
#endif
  // execute bench with NUM_THREADS threads
  for(wid = 0; wid < NUM_THREADS; wid++) { 
    if (pthread_create(&threads[wid], NULL, thread_run, (void *)(&tw[wid]))) {
      printf("ERROR: pthread_create() wid = %d \n", wid);
      exit(-1);
    }
  }    

  for(wid = 0; wid < NUM_THREADS; wid++) {
    void * status;
    if (pthread_join(threads[wid], &status)) {
      printf("ERROR: pthread_join() wid = %d \n", wid);
      exit(-1);
    }
  }
  // check time
#if defined(CPUTIME_ON_THREAD_RUSAGE)
  int ms2U = 0; 
  int ms2S = 0;
  for(wid=0; wid<NUM_THREADS; wid++) {
    ms2U = ms2U + tw[wid].execUTime;
    ms2S = ms2S + tw[wid].execSTime;
  }  
  printf(" %d_threads = %d  utime = %d stime = %d avg = %d avg_utime = %d avg_stime = %d ", NUM_THREADS, ms2U+ms2S, ms2U, ms2S, (ms2U+ms2S) / NUM_THREADS, ms2U/NUM_THREADS, ms2S/NUM_THREADS);
#elif defined (CPUTIME_ON_THREAD_DAYTIME)
  int max_ms = 0;
  int min_ms = 0;
  int sum_ms = 0;
  for(wid=0; wid<NUM_THREADS; wid++) {
    int ms = (int)(1000000*(tw[wid].execEndTime.tv_sec - tw[wid].execStartTime.tv_sec) + tw[wid].execEndTime.tv_usec - tw[wid].execStartTime.tv_usec) / 1000;
    if (ms > max_ms)
	max_ms = ms;    
    if (min_ms == 0 || ms < min_ms)
	min_ms = ms;
    sum_ms += ms;
  }  
  int  ms_single = (int)(1000000*(tw_single.execEndTime.tv_sec - tw_single.execStartTime.tv_sec) + tw_single.execEndTime.tv_usec - tw_single.execStartTime.tv_usec) / 1000;

  float avg_ms = (float)sum_ms / (float)NUM_THREADS;

  printf(" %d_threads min/avg/max = %d/%0.2f/%d ratio = %0.2f/%0.2f/%0.2f\n", NUM_THREADS, min_ms,avg_ms, max_ms, (float)ms_single/(float)min_ms, (float)ms_single/(float)avg_ms, (float)ms_single/(float)max_ms);  
  for(wid=0; wid < NUM_THREADS; wid++) {
    int ms = (int)(1000000*(tw[wid].execEndTime.tv_sec - tw[wid].execStartTime.tv_sec) + tw[wid].execEndTime.tv_usec - tw[wid].execStartTime.tv_usec) / 1000;    
    printf("wid = %d time = %d \n",wid, ms);  
  }
#else
  gettimeofday(&tv2, NULL); 
  int ms1 = (int)(1000000*(tv2.tv_sec - tv1.tv_sec) + tv2.tv_usec - tv1.tv_usec) / 1000;
  printf(" %d_threads = %d  ", NUM_THREADS, ms1);
#endif   
  fclose(FP);
  //  flushAndFreeHash(fresult_hash);

  dic_show_state(fresult_hash);
  //dic_show_delete_pool(fresult_hash);
  //dic_show_statistics(fresult_hash); // dic_show_statistics("stdout")

  dic_kill_env();


  return (0);
}


