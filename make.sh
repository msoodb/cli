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
    cc -o cli cli.c -Wall -Wextra -pedantic
    cc -o server server.c -Wall -Wextra -pedantic -lpthread
    cc -o client client.c -Wall -Wextra -pedantic
    cc -o file file.c -Wall -Wextra -pedantic
    cc -o list list.c 
    cc -o process process.c
    cc -o thread thread.c -lpthread
fi
