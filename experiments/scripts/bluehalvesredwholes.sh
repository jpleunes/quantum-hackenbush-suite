#!/bin/bash

# get number of blue halves x and number of red wholes y
x=$1
y=$2
# get ruleset (classical a, b, c, cprime, or d)
ruleset=$3
# get output file name
output="experiments/results/${ruleset}${x}bh${y}rw.csv"

for i in $(seq 0 $x)
do
    for j in $(seq 0 $y)
    do
        res=$(./build/quantum-hackenbush-suite $i 0 0 $j $ruleset)
        # append to output (replace newline with comma)
        $(echo "$res" | tr '\n' ',' >> $output)
    done
    # append newline
    echo "" >> $output
done
