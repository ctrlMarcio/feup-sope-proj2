#!/bin/bash

BASEDIR=$(dirname "$0")
cd "$BASEDIR"

fifoname="fifoname"

while true; do
    server_seconds=$(($RANDOM % 60 + 1))
    client_seconds=$(($RANDOM % 60 + 1))
    bathroom_places=$(($RANDOM % 100 + 1))
    thread_places=$(($RANDOM % 100 + 1))

    echo "Running test for Qs=$server_seconds, Us=$client_seconds, l=$bathroom_places and n=$thread_places"
    echo "Running test for Qs=$server_seconds, Us=$client_seconds, l=$bathroom_places and n=$thread_places" >> results.txt
    ./test_runner.sh $server_seconds $client_seconds $bathroom_places $thread_places $fifoname >> results.txt & pid=$! 
    
    wait "$pid"

done

