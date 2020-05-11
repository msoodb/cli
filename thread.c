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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


/*
 * https://computing.llnl.gov/tutorials/pthreads/
 * http://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/multi-thread.html
 * https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
 * https://www.thegeekstuff.com/2012/03/linux-threads-intro/
 */


int pwr(int a, int b)
{
	int i, p;
	p = 1;
	for (i = 0; i < b; ++i) {
		p *= a; 
	}

	return p;
}


void *t_func(void *value)
{
	printf("%s\n", "thread started...");

	printf("%d\n", (int) pthread_self());

	sleep(3);
	
	printf("%d\n", pwr(4, 3));
}

int main(int argc, char *argv[])
{

	pthread_t tid;

	printf("%s\n", "Before thread");

	pthread_create(&tid, NULL, t_func, NULL);
	pthread_join(tid, NULL);

	printf("%s\n", "After thread");

	
	return 0;
}
