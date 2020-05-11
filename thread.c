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



#define THREAD_COUNT 5

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
}

int main(int argc, char *argv[])
{
	pthread_t tid[THREAD_COUNT];
	int err;

	struct user *current;
	current = malloc(sizeof(struct user) * 1);

	int i;
	for (i = 0; i < THREAD_COUNT; ++i) {
		current->id = i;
		current->name = "masoud";
		current->addr = "Esspo";
		current->status = true;
		err = pthread_create(&tid[i], NULL, user_register, (void *)current);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
	}

	for (i = 0; i < THREAD_COUNT; ++i) {
		pthread_join(tid[i], NULL);
	}

	return 0;
}
