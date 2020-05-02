#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <tQ> <tU> <fifoname>"
    exit 1
fi

(rm q1.err & rm q1.log & rm u1.err & rm u1.log) 2> /dev/null # they are overwritten below, but better safe than sorry

../bin/Q1 -t $1 $3 > q1.log 2> q1.err & pid1=$!
../bin/U1 -t $2 $3 > u1.log 2> u1.err & pid2=$!

wait $pid1
wait $pid2

n2LATE=`grep 2LATE q1.log | wc -l` ; echo "2LATE: $n2LATE"
nCLOSD=`grep CLOSD u1.log | wc -l` ; echo "CLOSD: $nCLOSD"

if [ $n2LATE -eq $nCLOSD ]; then
    echo "OK!"
else
    echo "FAILED!"
fi