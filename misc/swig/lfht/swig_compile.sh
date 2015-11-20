#!/bin/bash


del swig_working_dir/*

cp *.[i,h,c] swig_working_dir/
#cp *.java swig_working_dir/

cd swig_working_dir/ 

#swig -java -package lixo lfht.i
swig -java lfht.i



# Machines Desktop and Toshiba
#gcc -fpic -c lfht.h lfht_wrap.c -I/home/miguel/bin/jdk1.7.0_25/include/  -I/home/miguel/bin/jdk1.7.0_25/include/linux/ 

# Machine Leap
gcc -fpic -c -c lfht.h lfht_wrap.c  -I/home/miguelareias/java-hash/jdk1.7.0_25/include/  -I/home/miguelareias/java-hash/jdk1.7.0_25/include/linux/

gcc -shared lfht_wrap.o -o liblfht.so

#javac RunMe.java
#java RunMe

#check the tutorial at:
#http://nextmidas.techma.com/nxm360/htdocs/usersguide/Swig.html
#or the file swig.pdf
