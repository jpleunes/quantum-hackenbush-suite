#!/bin/bash

# get the analysis function we want to use (outcome, value, or birthday)
function=$1
# get number of legs n
n=$2
# get flavour (classical a, b, c, cprime, or d)
flavour=$3
# get output file name
output="experiments/results/${function}/${flavour}${n}legs.csv"

for i in $(seq 0 $n)
do
    res=$(./build/quantum-hackenbush-suite ${function} circusTent_${i} $flavour)
    # append to output (replace newline with comma)
    $(echo "$res" | tr '\n' ',' >> $output)
done
# append newline
echo "" >> $output
