#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


void char_at_a_time(const char *str)
{
	while( *str != '\0' ) {
		putchar(*str++);
		fflush(stdout);
		usleep(50);
	}
}

int main(int argc, char *argv[])
{
	pid_t p;

	int i;
	
	printf("main process id = %d \nparent process id = %d\n\n",
	       (int) getpid(), (int) getppid());


	/*char *arg_list[] = {"ls", "-1", NULL};
	  execvp ("ls", arg_list);*/
	
	sleep(3);
	p = fork();

	if (p < 0) {
		printf("%s\n", "Error!");
		return 0;
	}
	if (p == 0) {		
		
		printf("%d %s\n", i, " Child");
		printf("process id: %d parent process id: %d\n\n",
		       (int) getpid(), (int) getppid());
		
		char_at_a_time(".............\n");

		
		printf("\n%s\n", "Child exiting...");
		exit(0);
	}

	i = 100;

	printf("%d %s\n", i, " Parent:");
	printf("process id: %d parent process id: %d\n",
	       (int) getpid(), (int) getppid());
	printf("my child process id: %d\n\n", (int) p);

	wait(NULL);
	char_at_a_time("|||||||||||||||");

	for (i = 0; i < 10; ++i) {
		printf("%s", "a");
	}
	printf("%s", "xxxx");
	printf("\n");
	

	/*fork();
	fork();
	printf("process id: %d parent process id: %d\n\n",
	(int) getpid(), (int) getppid());*/

	return 0;
}
