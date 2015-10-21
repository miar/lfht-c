#include "threads.h"

void flushAndFreeHash(const char *fout) {
  
#ifdef FLUSH_HASH_STATISTICS
  total_nodes = total_buckets = total_empties = total_max_nodes = 0;
  total_min_nodes = DATASET_SIZE;
#endif // FLUSH_HASH_STATISTICS
  FP = fopen(fout, "w"); 
  free_bucket_array((ans_node_ptr *)TAG(AnsHash_buckets(HN)), 0);
#ifdef FLUSH_HASH_STATISTICS
  fprintf(FP, "------------------------------------------------------------------\n");
  fprintf(FP, "  Nr of nodes           = %d                               \n", total_nodes);
  fprintf(FP, "  Nr of buckets         = %d                               \n", total_buckets);  
  fprintf(FP, "  Nr of empty buckets   = %d                               \n", total_empties);
  fprintf(FP, "  Max nodes (non empty) = %d (MAX_NODES_PER_BUCKET = %d)   \n", total_max_nodes, MAX_NODES_PER_BUCKET);
  fprintf(FP, "  Min nodes (non empty) = %d                               \n", total_min_nodes);
  fprintf(FP, "  Avg nodes per bucket (total) = %3.2f  (non empty only) = %3.2f \n", (float)(total_nodes)/(total_buckets), (float) (total_nodes)/(float)(total_buckets-total_empties));
  fprintf(FP, "------------------------------------------------------------------\n");
#endif // FLUSH_HASH_STATISTICS
  
  fclose(FP); 
  FREE_BLOCK(HN);
  return;
}

void * thread_run(void *ptr) { 
  struct thread_work *tw;
  tw = (struct thread_work *) ptr;
  long i;
#ifdef MIGS_BUFFER_ALLOC
  struct_buffer_ptr sb;
  ALLOC_STRUCT_BUFFER(sb);
#endif /* MIGS_BUFFER_ALLOC */  

#ifdef SET_THREAD_AFFINITY
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET((tw->wid)*CPU_LAUNCHER_OFFSET, &cpuset); 
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
  for (i = s; i < e; i++)  
#ifdef MIGS_BUFFER_ALLOC
    check_insert(dataSet[i], sb);
#else
    check_insert(dataSet[i]);
#endif /* MIGS_BUFFER_ALLOC */  

#if defined(CPUTIME_ON_THREAD_RUSAGE)
  getrusage(RUSAGE_THREAD, &tv2);
  tw->execUTime = (int)(((tv2.ru_utime.tv_sec  - tv1.ru_utime.tv_sec)  * 1000000 +
          (tv2.ru_utime.tv_usec - tv1.ru_utime.tv_usec)) / 1000); 
  tw->execSTime = (int)(((tv2.ru_stime.tv_sec  - tv1.ru_stime.tv_sec)  * 1000000 +
          (tv2.ru_stime.tv_usec - tv1.ru_stime.tv_usec)) / 1000);  
#elif defined (CPUTIME_ON_THREAD_DAYTIME)
  gettimeofday(&(tw->execEndTime), NULL); 
#endif
#ifdef MIGS_BUFFER_ALLOC
  FREE_STRUCT_BUFFER(sb);
#endif  /* MIGS_BUFFER_ALLOC */  
  pthread_exit(NULL);
}


void createTestAndSolution(void){  
  Term i;
#ifdef CREATE_NEW_DATA_SET_RANDOM
  /* create data set file */
  FP = fopen(fdata_set, "w");
  srand(time(NULL)); /* initialize the random seed */
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
  // create the solution of hash table using a single thread
  new_answer_trie_hash_node();   
  // execute bench with one thread 
  wait_ = 1;
#if defined(CPUTIME_ON_THREAD_RUSAGE) || defined(CPUTIME_ON_THREAD_DAYTIME)  
  pthread_t threads;  
  tw_single.wid = 0;
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
  //printf("********************SINGLE THREAD FINISHED**********\n");
#if defined(CPUTIME_ON_THREAD_RUSAGE)
  printf(" Cputime RUSAGE THREAD (milliseconds): 1_thread = %d ", tw_single.execUTime + tw_single.execSTime); 
#else
  printf(" Cputime DAYTIME THREAD (milliseconds): 1_thread = %d ",(int)(1000000*(tw_single.execEndTime.tv_sec - tw_single.execStartTime.tv_sec) + tw_single.execEndTime.tv_usec - tw_single.execStartTime.tv_usec) / 1000); 
#endif
#else  
  // check time 
  struct timeval tv1, tv2;  
  gettimeofday(&tv1, NULL);   
  for (i = 0 ; i < DATASET_SIZE ; i++)
    check_insert(dataSet[i]);
  // check time 
  gettimeofday(&tv2, NULL);
  int ms = (int)(1000000*(tv2.tv_sec - tv1.tv_sec) + tv2.tv_usec - tv1.tv_usec) / 1000;
  printf(" Cputime DAYTIME MAIN (milliseconds): 1_thread = %d ", ms);     
#endif
  flushAndFreeHash(fcorrect_hash);
#endif /* SINGLE_THREAD_EXECUTION */
}


int main() {
  pthread_t threads[NUM_THREADS];  
  int wid = 0;
  createTestAndSolution();
  /* run test */  
  new_answer_trie_hash_node(); 
  FP = fopen(fdata_set, "r");
  struct thread_work tw[NUM_THREADS];  
#ifdef DIVIDE_WORK_BETWEEN_THREADS
  int offset = (int)(DATASET_SIZE / NUM_THREADS);
  int startI = 0;
  do { 
    tw[wid].wid = wid;
    tw[wid].startI = startI;
    tw[wid].endI = startI + offset;	
    startI = tw[wid].endI;
    wid++;
  } while(wid < NUM_THREADS);    
#else
  do {
    tw[wid].wid = wid;
    tw[wid].startI = 0;
    tw[wid].endI = DATASET_SIZE;	
    wid++;
  } while(wid < NUM_THREADS);
#endif  
  /* check time */    
  wait_ = NUM_THREADS;
  ///////////////////////////////////////////////////////////////
  //  return 0 ;                                                   //
  ///////////////////////////////////////////////////////////////

#if !defined(CPUTIME_ON_THREAD_RUSAGE) && !defined(CPUTIME_ON_THREAD_DAYTIME)
  struct timeval tv1, tv2;  
  gettimeofday(&tv1, NULL); 
#endif
  /* execute bench with NUM_THREADS threads */
  for(wid=0; wid<NUM_THREADS; wid++) { 
    if (pthread_create(&threads[wid], NULL, thread_run, (void *)(&tw[wid]))) {
      printf("ERROR: pthread_create() wid = %d \n", wid);
      exit(-1);
    }
  }    

  for(wid=0; wid<NUM_THREADS; wid++) {
    void * status;
    if (pthread_join(threads[wid], &status)) {
      printf("ERROR: pthread_join() wid = %d \n", wid);
      exit(-1);
    }
  }
  /* check time */
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
  flushAndFreeHash(fresult_hash);	    
  exit(0);
}
