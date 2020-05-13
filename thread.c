/*
 * Copyright (c) 2020-2020 msoodb.org
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <pthread.h>


/*
 * creation
 * termination
 * synchronization (joins,blocking)
 * scheduling 
 * data management
 * process interaction
 */


#define COUNT_TO 10000000
#define MAX_CORES 12

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long long counter = 0;


struct user
{
	int id;
	char *name;
	char *addr;
	bool status;
};

void *user_register(void *ptr)
{
	struct user *t_user = (struct user *)ptr;
	sleep(1);
	printf("%d: %s\n", (int) pthread_self(), "thread started...");
	printf("\tid: %d, name: %s, addr: %s, status: %d\n",
	       t_user->id,
	       t_user->name,
	       t_user->addr,
	       t_user->status);
}

void *count(void *arg)
{
	for (;;) {

		/* Start critical section */
		pthread_mutex_lock(&mutex);
		
		if (counter >= COUNT_TO) {
			pthread_mutex_unlock(&mutex);
			return NULL;			
		}

		counter++;

		/* End critical section */
		pthread_mutex_unlock(&mutex);

		printf("counter = %lld\n", counter);
	}

}

void *counter_increase(void *arg)
{
	printf("%s: %lld\n", "Thread Number", (long)pthread_self());
	/* Start critical section */
	pthread_mutex_lock(&mutex);
		
	counter++;

	/* End critical section */
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
	int i, err;
	pthread_t *thread_pool;
	pthread_attr_t attr;
	struct user *current;

	thread_pool = malloc(sizeof(pthread_t) * MAX_CORES);
	pthread_attr_init(&attr);
	current = malloc(sizeof(struct user) * 1);

	/* Spawn threads */
	for (i = 0; i < MAX_CORES; ++i) {
		/*current->id = i;
		current->name = "masoud";
		current->addr = "Esspo";
		current->status = true;
		err = pthread_create(&thread_pool[i], NULL, user_register, (void *)current);*/
		err = pthread_create(&thread_pool[i], &attr, counter_increase, NULL);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
	}

	/* Wait for threads to finish */
	for (i = 0; i < MAX_CORES; ++i) {
		pthread_join(thread_pool[i], NULL);
	}

	printf("%lld\n", counter);
	return 0;
}
