#!/bin/bash
SIZE=100

for i in `seq 1 5`;
    do
            ./test_conc $i*$SIZE $SIZE $i& 
    done 

