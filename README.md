
# THIS PROJECT IS NO LONGER VALID!!!
# Check arenac project.



# cli program
C command line application template.

> cli \
> server \
> client \
> list \
> process \
> thread \
> file


### cli.c            
Command line arguments and options management.                                        

### server.c         
Socket server template.

### client.c          
Socket client template.

## list.c
Simple doubly linked list implementation.

## process.c
Multi-Process implementation.

## thread.c
Multi-Thread implementation.

## file.c
Read and Write file, Read line by line, implementation.

### Makefile
Containing a set of directives used by \
a make build automation tool.  

# build                 
```sh                
$ cc -o cli cli.c
$ cc -o server server.c -lpthread
$ cc -o client client.c
$ cc -o list list.c
$ cc -o process process.c
$ cc -o thread thread.c -lpthread
$ cc -o file file.c
```
