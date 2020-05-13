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
#define _MAXIMUM_CLIENT 99
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/*
 * Handle connection for each client
*/
void *connection_handler(void *arg)
{		
	char recieve_buffer[_BUFFER_SIZE] = {0};
	char *send_buffer;
	int sock;

	sock = *(int *)arg;
	if (arg != NULL) free(arg);
	
	/* Start critical section */
	pthread_mutex_lock(&mutex);

	/* Read and Write loop */
	while(1){
		/* Read */
		memset(recieve_buffer, '\0', sizeof recieve_buffer);
		if( read(sock, recieve_buffer, _BUFFER_SIZE) < 0){
			pthread_mutex_unlock(&mutex);
			return NULL;
		}

		/* write */
		send_buffer = concat("server: ", recieve_buffer);
		if( write(sock, send_buffer, strlen(send_buffer)) < 0){		
			pthread_mutex_unlock(&mutex);
			return NULL;
		}
		if (send_buffer != NULL) free(send_buffer);
	}

	/* End critical section */
	pthread_mutex_unlock(&mutex);
			
	return NULL;	
}

int main(int argc, char *argv[])
{	
	int sockfd;
	int sock;
	
	int client_len;
	int port;

	pthread_t socket_thread;

	struct sockaddr_in server;
	struct sockaddr_in client;
	char *client_ip;
	int client_port;

	char recieve_buffer[_BUFFER_SIZE];
	char *send_buffer;

	
	sockfd = -1;
	sock = -1;
	port = 4545;

	/* socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		exit (EXIT_FAILURE);	
	
	int reuse = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
		exit (EXIT_FAILURE);

	/* bind */
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
		exit (EXIT_FAILURE);

	/* listen */
	listen(sockfd, _MAXIMUM_CLIENT);
	
	while(1){
		/* accept */
		client_len = sizeof(struct sockaddr_in);		
		sock = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&client_len);
		if (sock < 0)
			exit (EXIT_FAILURE);		

		/* Client IP and PORT */
		client_ip = inet_ntoa(client.sin_addr);
		client_port = ntohs(client.sin_port);
		printf("%d: %s %d\n", sock, client_ip, client_port);
				
		/* Read and Write in thread */		
		if( pthread_create(&socket_thread, NULL, connection_handler, (void*)&sock) < 0){
			printf("%s\n", "Could not create thread");
			return EXIT_FAILURE;
		}

		pthread_join(socket_thread, NULL);

		/* close */
		if (client_ip != NULL) free(client_ip);
		if (sock) close(sock);			
	}

	return EXIT_SUCCESS;
}
