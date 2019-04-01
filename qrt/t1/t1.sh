#!/bin/bash

# Tests for mod 3.
# Mod 3 sequnce is 0 1 1 2 0 2 2 1. Pisano period = 8.*/
mod3_seq=(0 1 1 2 0 2 2 1)
mod3_period=8

for i in `seq 0 15`; do
    res=`./t1 $i 3`
    seq_val=${mod3_seq[$i % $mod3_period]}

    [ $res -eq $seq_val ] || { printf "F(%d) mod 3 != %d\n" $i $seq_val; exit -1; };
done
