#!/bin/bash

# get the analysis function we want to use (outcome, value, or birthday)
function=$1
# get number of blue halves x and number of red wholes y
x=$2
y=$3
# get flavour (classical a, b, c, cprime, or d)
flavour=$4
# get output file name
output="experiments/results/${function}/${flavour}${x}bh${y}rw.csv"

for i in $(seq 0 $x)
do
    for j in $(seq 0 $y)
    do
        res=$(./build/quantum-hackenbush-suite ${function} shortHollyhocks_${i}_0_0_${j} $flavour)
        # append to output (replace newline with comma)
        $(echo "$res" | tr '\n' ',' >> $output)
    done
    # append newline
    echo "" >> $output
done
