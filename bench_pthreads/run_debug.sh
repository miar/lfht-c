#!/bin/sh

LD_PRELOAD='/home/miguelareias/conferences/iclp2013/jemalloc-3.1.0/lib/libjemalloc.so'
N_TESTS=$1 
if test -z "${1}"; then
    N_TESTS=1
fi
error_compile=0
gcc -Wall -pthread -O3 migshash.c threads.c || error_compile=1

## -g for debugging -O0 to remove all optimizations
#gcc -Wall -pthread -O0 -g migshash.c threads.c || error_compile=1
if test ${error_compile} -eq 1; then
    exit 1
fi

del output/correct_hash output/result_hash

while [ ${N_TESTS} -ge 1 ]
do
    echo -n "Test Nr " ${N_TESTS} " : " 
    error_execution=0
    ./a.out || error_execution=1
    echo -n "| Debug: "
    if test ${error_execution} -eq 1; then
	echo " Fail. Got a Segmentation Fault" 
	echo " Action: check data_set file. Remove CREATE_NEW_DATA_SET flag from main.c to launch the same test"
	exit 1
    fi

    python run_output_filter.py output/correct_hash
    python run_output_filter.py output/result_hash
    error_result=0
    diff output/correct_hash.sorted output/result_hash.sorted > output/result.diff || error_result=1
    
    if test ${error_result} -eq 1; then
	echo " Fail. Got a Results Error" 
	echo " Actions: check data_set and result.diff files. Remove CREATE_NEW_DATA_SET flag from main.c to launch the same test"
	exit 1
    fi
    N_TESTS=`expr ${N_TESTS} - 1`
    echo " Ok "
done


exit 0

