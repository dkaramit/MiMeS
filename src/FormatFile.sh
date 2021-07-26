#this script takes a file, removes empty lines, sorts it, removes any duplicates, and replaces it!

#!/bin/bash

tmp1=.tmp1.xtx
tmp2=.tmp2.xtx

sed '/^\s*$/d' $1 > $tmp1
LC_ALL=C sort --general-numeric-sort -u $tmp1 > $tmp2

cat $tmp2 > $tmp1

nLines=$(awk 'END{print NR}' $tmp1)
awk -v nLines=$nLines 'NR!=nLines {printf "%s\n", $0}; NR==nLines {printf "%s", $0};' $tmp1 > $1

rm $tmp1
rm $tmp2
