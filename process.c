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

void count_print(char *name)
{
	const int COUNT_NUM = 5;

	int i;
	for (i = 0; i < COUNT_NUM; ++i) {
		sleep(rand() % 5);
		printf("%s: %s %d\n", "Done pass for", name, i);
	}

}

int main(int argc, char *argv[])
{
	pid_t p;

	int i;
	
	printf("main process id = %d \nparent process id = %d\n\n",
	       (int) getpid(), (int) getppid());

	sleep(3);

	p = fork();

	if (p < 0) {
		printf("%s\n", "Error!");
		return 0;
	}
	if (p == 0) {		
		printf("%s\n", "Child starting...");

		count_print("Child");
		
		printf("\n%s\n", "Child exiting...");
		exit(4);
	}



	//count_print("Parent");

	/*int child_status;
	pid_t child_pid;
	child_pid = wait(&child_status);
	
	printf("%s: %d %s: %d\n", "Finish Child",
	       (int) child_pid, "with Status",
	       WEXITSTATUS(child_status));
	       printf("%s\n", "Parent exiting...");*/

	sleep(60);
	
	return 0;
}
