#!/bin/bash

if [ "$#" -ne 5 ]; then
    echo "Usage: $0 <tQ> <tU> <lQ> <nQ> <fifoname>"
    exit 1
fi

BASEDIR=$(dirname "$0")
cd "$BASEDIR"

make --directory=.. > /dev/null 2> /dev/null

if [ "$?" -eq 2 ]; then
    echo "Fatal error. Could not compile..."
    exit 2
fi

(rm q2.err && rm q2.log && rm u2.err && rm u2.log) 2> /dev/null # they are overwritten below, but better safe than sorry

../Q2 -t "$1" -l "$3" -n "$4" "$5" > q2.log 2> q2.err & pid1=$!
../U2 -t "$2" "$5" > u2.log 2> u2.err & pid2=$!

wait "$pid1"
wait "$pid2"

n2LATE=$(grep 2LATE q2.log | wc -l) ; echo "2LATE: $n2LATE"
nCLOSD=$(grep CLOSD u2.log | wc -l) ; echo "CLOSD: $nCLOSD"

if [ "$n2LATE" -eq "$nCLOSD" ]; then
    echo "OK!"
else
    echo "FAILED!"
fi
