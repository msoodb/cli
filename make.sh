#!/bin/bash
ARG=$1

if [[ $ARG == "clean" ]]
then
    rm cli
    rm server
    rm client
    rm list
    rm process
    rm thread
    rm file
else
    cc -o cli cli.c
    cc -o server server.c -lpthread
    cc -o client client.c
    cc -o list list.c
    cc -o process process.c
    cc -o thread thread.c -lpthread
    cc -o file file.c
fi
