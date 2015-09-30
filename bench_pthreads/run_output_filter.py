##################################################################
##
## File:      run_output_filter.py
## Author(s): 
## Program:   Script for sorting the output
##
##################################################################

import sys
import os
import operator

########## parse line ##########

fout = open(sys.argv[1] + '.sorted', 'w')
fin =  open(sys.argv[1], 'r') 

line = fin.readline()

while (line):
    if (line[0:5] == "entry" or line[0] =="|") :
        fout.write(line)
    else:
        lr = ""
        for val in sorted(line.split()) :
            if (val !=''):
                lr = lr + " " + val
        fout.write(lr+'\n')
    line = fin.readline()

fin.close()
fout.close()
