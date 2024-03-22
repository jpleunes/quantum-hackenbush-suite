#!/bin/bash

# get the analysis function we want to use (outcome, value, or birthday)
function=$1
# get number of blue halves x and number of red wholes y
x=$2
y=$3
# get ruleset (classical a, b, c, cprime, or d)
ruleset=$4
# get output file name
output="experiments/results/${function}/${ruleset}${x}bh${y}rw.csv"

for i in $(seq 0 $x)
do
    for j in $(seq 0 $y)
    do
        res=$(./build/quantum-hackenbush-suite $function $i 0 0 $j $ruleset)
        # append to output (replace newline with comma)
        $(echo "$res" | tr '\n' ',' >> $output)
    done
    # append newline
    echo "" >> $output
done
