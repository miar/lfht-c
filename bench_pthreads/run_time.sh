#!/bin/sh

#export LD_PRELOAD='/home/miguelareias/conferences/2012/icpads2012/other_memory_allocators/compiled_ma/libhoard.so' # this one freezes in the FREE_BLOCK(HASH) instruction
#export LD_PRELOAD='/home/miguelareias/conferences/iclp2013/jemalloc-3.1.0/lib/libjemalloc.so' ## import this file 

#export LD_PRELOAD='/home/miguelareias/c-hash/other_mem_alloc/lockless_allocator_linux/64bit/libllalloc.so.1.4' 

###best mem allocator is jemalloc

N_TESTS=$1 
if test -z "${1}"; then
    N_TESTS=1
fi
error_compile=0
gcc -Wall -lpthread -O3  threads.c || error_compile=1
#gcc -Wall -pthread -g migshash.c threads.c || error_compile=1

## -g for debugging and  -O0 to remove all optimizations
#gcc -Wall -pthread -g  migshash.c threads.c || error_compile=1
if test ${error_compile} -eq 1; then
    exit 1
fi

while [ ${N_TESTS} -ge 1 ]
do
    echo -n "Test Nr " ${N_TESTS} " -> " 
    error_execution=0 
    ./a.out || error_execution=1
     
    if test ${error_execution} -eq 1; then
	echo " Fail. Got a Segmentation Fault" 
	echo " Action: check data_set file. Remove CREATE_NEW_DATA_SET flag from main.c to launch the same test"
	exit 1
    fi
    
    error_result=0 
    diff output/correct_hash output/result_hash || error_result=1
    
    if test ${error_result} -eq 1; then
	echo " Fail. Got a Results Error" 
	echo " Actions: check data_set and result.diff files. Remove CREATE_NEW_DATA_SET flag from main.c to launch the same test"
	exit 1
    fi

    N_TESTS=`expr ${N_TESTS} - 1`    
    #echo " DEBUG -> OK (FLUSH_HASH_STATISTICS ONLY)"
done

exit 0



