/*
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
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

/* 
   ______________
  |              |
  |   socket()   |
  |______________|
         |
   ______V_______
  |              |
  |    bind()    |
  |______________|
         |
   ______V_______
  |              |
  |   listen()   |
  |______________|
         |
   ______V_______
  |              |
  |   accept()   |<------------------------------------
  |______________|                                     |
         |__________________________                   |
   ______V_______             ______V_______           |
  |              |           |              |          |
  |    read()    |<---       |    read()    |<---      |
  |______________|    |      |______________|    |     |
         |            |             |            |     |
   ______V_______     |       ______V_______     |     |
  |              |    |      |              |    |     |
  |    write()   |----       |    write()   |----      |
  |______________|           |______________|          |
         |__________________________|                  |
   ______V_______                                      |
  |              |                                     |
  |    close()   |-------------------------------------
  |______________|

Ref: 
 The Definitive Guide to Linux Network Programming.
 Keir Davis, John W. Turner, and Nathan Yocom.
*/

#define _BUFFER_SIZE 1024
#define _MAXIMUM_CLIENT 10

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *connection_handler(void *arg)
{
	char recieve_buffer[_BUFFER_SIZE] = {0};
	char *send_buffer;
	int sock;

	sock = *(int *)arg;
	
	printf("Thread number %ld\n", pthread_self());	
	
	/* start critical section */
	pthread_mutex_lock(&mutex);

	/* read and write loop */
	while(1){
		/* read */
		memset(recieve_buffer, '\0', sizeof recieve_buffer);
		if( read(sock, recieve_buffer, _BUFFER_SIZE) < 0){
			pthread_mutex_unlock(&mutex);
			pthread_exit(0);
		}

		//printf("%s\n", recieve_buffer);
		if (strcmp(recieve_buffer, "exit\n") == 0) {
			printf("%s\n", "exit");
			pthread_mutex_unlock(&mutex);
			if (arg != NULL) free(arg);
			pthread_exit(0);	
		}

		/* write */
		send_buffer = "-----------\n";
		if( write(sock, send_buffer, strlen(send_buffer)) < 0){		
			pthread_mutex_unlock(&mutex);
			pthread_exit(0);
		}
	}

	/* end critical section */
	pthread_mutex_unlock(&mutex);

	if (arg != NULL) free(arg);
	
	pthread_exit(0);
}


int main()
{		
	int sockfd;
	int sock;
	int *sock_ptr;
	
	int client_len;
	int port;

	pthread_t socket_thread;

	struct sockaddr_in server;
	struct sockaddr_in client;

	char *client_ip;
	int client_port;

	
	sockfd = -1;
	sock = -1;
	port = 4545;

	/* socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1){
		printf("%s\n", "Socket Failed!");
		exit (EXIT_FAILURE);
	}
	
	int reuse = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
	{
		printf("%s\n", "Setsocket opt Failed!");
		exit (EXIT_FAILURE);
	}

	/* bind */
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("%s\n", "Bind Failed!");
		exit (EXIT_FAILURE);
	}

	/* listen */
	listen(sockfd, _MAXIMUM_CLIENT);

	
	while(1){
		/* accept */
		client_len = sizeof(struct sockaddr_in);		
		sock = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&client_len);
		if (sock < 0){
			printf("%s\n", "Connection Failed!");
			exit (EXIT_FAILURE);
		}

		/* client IP and PORT */
		client_ip = inet_ntoa(client.sin_addr);
		client_port = ntohs(client.sin_port);
		printf("%s %s:%d\n", "Connection stablished to", client_ip, client_port);
		
		/* read and write */
		sock_ptr = malloc(sizeof(int) * 1);
		*sock_ptr = sock;
		if( pthread_create(&socket_thread, NULL, connection_handler, (void*)sock_ptr) < 0){
			printf("%s\n", "Thread Creation Faild!");
			return EXIT_FAILURE;
		}

		//pthread_detach(socket_thread);
		pthread_join(socket_thread, NULL);
		
                /* close */		
		if (sock) close(sock);	
	}

	return EXIT_SUCCESS;
}
