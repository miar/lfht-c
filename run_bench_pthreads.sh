#!/bin/sh

# 'libjemalloc.so' not in use for now.

#LD_PRELOAD='/home/miguelareias/conferences/iclp2013/jemalloc-3.1.0/lib/libjemalloc.so'

N_TESTS=$1 

if test -z "${1}"; then
    N_TESTS=1
fi

error_compile=0

gcc  -Wall -Wmissing-prototypes -Wmissing-declarations bench_pthreads.c -lpthread -g -o bench.out || error_compile=1

if test ${error_compile} -eq 1; then
    exit 1
fi

del output/lfht_correct output/lfht_result

while [ ${N_TESTS} -ge 1 ]
do
    echo -n "Test Nr " ${N_TESTS} " : " 
    error_execution=0
    ./bench.out || error_execution=1
    echo -n "| Debug: "
    if test ${error_execution} -eq 1; then
	echo " Fail. Got a Segmentation Fault" 
	echo " Action: check data_set file. Remove CREATE_NEW_DATA_SET flag from main.c to launch the same test"
	exit 1
    fi

    python run_output_filter.py output/lfht_correct
    python run_output_filter.py output/lfht_result
    error_result=0
    diff output/lfht_correct.sorted output/lfht_result.sorted > output/result.diff || error_result=1
    
    if test ${error_result} -eq 1; then
	echo " Fail. Got a Results Error" 
	echo " Actions: check data_set and result.diff files. Remove CREATE_NEW_DATA_SET flag from main.c to launch the same test"
	exit 1
    fi
    N_TESTS=`expr ${N_TESTS} - 1`
    echo " Ok "
done

exit 0


# # compile on with gcc 

# rm bench.out
# del output/lfht_correct output/lfht_result

# gcc  -Wall -Wmissing-prototypes -Wmissing-declarations bench_pthreads.c -lpthread -g -o bench.out

# #compile and check assembly code

# #gcc -c -g -Wa,-a,-ad -lpthread bench_pthreads.c -o bench.out > foo.lst

# # compile on with gcc 
# #clang -Wall -lpthread bench_pthreads.c -o bench.out

# #and run for the moment...

# ./bench.out

# #tkdiff output/correct_hash output/result_hash
