#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <tQ> <tU> <fifoname>"
    exit 1
fi

rm -rf test;
mkdir test;

./bin/Q1 -t $1 $3 > test/q1.log 2> test/q1.err & pid1=$!
./bin/U1 -t $2 $3 > test/u1.log 2> test/u1.err & pid2=$!

wait $pid1
wait $pid2

n2LATE=`grep 2LATE test/q1.log | wc -l` ; echo $n2LATE
nCLOSD=`grep CLOSD test/u1.log | wc -l` ; echo $nCLOSD

if [ $n2LATE -eq $nCLOSD ]; then
    echo "OK!"
else
    echo "FAILED!"
fi