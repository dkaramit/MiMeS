#!/bin/bash

#This script reads a file (whihc is passed as argument) with the inputs for Axion.run, 
# and prints the result. 
# Also, in stderr, print the time it took (is seconds) to run it.

time (cat $1 | xargs ./Axion.run) 2> .time

sed 's/m/ /g; s/\,/./g; s/s//g' .time | awk '$1 ~ /real/ {print 60*$2+$3}' >&2
 

rm -f .time
