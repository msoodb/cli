#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t child_pid;

	printf("main process id = %d (parent process id = %d)\n\n",
	       (int) getpid(), (int) getppid());

	child_pid = fork();

	printf("%s child_pid: %d process id: %d parent process id: %d\n\n",
	       "hello", child_pid, (int) getpid(), (int) getppid());

	
	return 0;
}
