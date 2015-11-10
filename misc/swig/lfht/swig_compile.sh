#!/bin/bash

del swig_working_dir/*

cp *.[i,h,c] swig_working_dir/

cd swig_working_dir/ 

swig -java -includeall lfht.i

gcc -fpic -c lfht.h lfht_wrap.c -I/home/miguel/bin/jdk1.7.0_25/include/  -I/home/miguel/bin/jdk1.7.0_25/include/linux/ 

gcc -shared lfht_wrap.o -o liblfht.so



#check the tutorial at:
#http://nextmidas.techma.com/nxm360/htdocs/usersguide/Swig.html
#or the file swig.pdf
