compile:
	swig  -java example.i
# Machines Desktop and Toshiba
#	gcc -fpic -c example.c example_wrap.c  -I/home/miguel/bin/jdk1.7.0_25/include/  -I /home/miguel/bin/jdk1.7.0_25/include/linux/
# Machine Leap 
	gcc -fPIC -c example.c example_wrap.c  -I/home/miguelareias/java-hash/jdk1.7.0_25/include/  -I/home/miguelareias/java-hash/jdk1.7.0_25/include/linux/
	gcc -shared example.o example_wrap.o -o libexample.so
	javac UseExample.java
	java UseExample
#	make clean
clean:
	del exampleJNI.java example.o example_wrap.* example.java *.class libexample.so



#check tutorial at:
#http://nextmidas.techma.com/nxm360/htdocs/usersguide/Swig.html
#or file swig.pdf
