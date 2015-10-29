#include "bench_pthreads.h"

void createTestAndSolution(void){  
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
  // create the solution of hash table using a single thread
  ///////////////////////////  new_answer_trie_hash_node();      HHHHHHHHHEEERRREEEE  
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
    //////////////    check_insert(dataSet[i]);

  // check time 
  gettimeofday(&tv2, NULL);
  int ms = (int)(1000000*(tv2.tv_sec - tv1.tv_sec) + tv2.tv_usec - tv1.tv_usec) / 1000;
  printf(" Cputime DAYTIME MAIN (milliseconds): 1_thread = %d ", ms);     
#endif
  ///////////////////flushAndFreeHash(fcorrect_hash);
#endif /* SINGLE_THREAD_EXECUTION */

  return;

}


int main() {
  pthread_t threads[NUM_THREADS];  
  int wid = 0;
  createTestAndSolution();

  /*
  // run test
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
  // check time
  wait_ = NUM_THREADS;

#if !defined(CPUTIME_ON_THREAD_RUSAGE) && !defined(CPUTIME_ON_THREAD_DAYTIME)
  struct timeval tv1, tv2;  
  gettimeofday(&tv1, NULL); 
#endif
  // execute bench with NUM_THREADS threads
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
  flushAndFreeHash(fresult_hash);	    
  return (0);
  */
}


