 /*.
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
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define _BUFFER_SIZE 1024
#define _MAXIMUM_CLIENT 99


int main(int argc, char *argv[])
{	
	int sockfd;
	int sock;
	
	int client_len;
	int port;

	struct sockaddr_in server;
	struct sockaddr_in client;

	char *client_ip;
	int client_port;

	char recieve_buffer[_BUFFER_SIZE] = {0};
	char *send_buffer;

	
	sockfd = 0;
	sock = 0;
	port = 4545;

	/* Create socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		exit (EXIT_FAILURE);	

	/* Reuse port */
	int reuse = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
		exit (EXIT_FAILURE);

	/* Create socket */
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
		exit (EXIT_FAILURE);

	/* Listen */
	listen(sockfd, _MAXIMUM_CLIENT);
	
	while(1)
	{
		/* Accept and incoming connection */
		client_len = sizeof(struct sockaddr_in);		
		sock = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&client_len);
		if (sock < 0)
			exit (EXIT_FAILURE);		

		client_ip = inet_ntoa(client.sin_addr);
		client_port = ntohs(client.sin_port);		
		
		/* Recieve data from client */
		memset(recieve_buffer, '\0', sizeof recieve_buffer);
		if( recv(sock, recieve_buffer, _BUFFER_SIZE, 0) < 0)
			break;		

		/* Send data to client */
		send_buffer = "hello, socket server.";
		if( send(sock, send_buffer, strlen(send_buffer), 0) < 0)
			exit (EXIT_FAILURE);		

		/* Close socket */
		close(sock);
		shutdown(sock, 0);
		shutdown(sock, 1);
		shutdown(sock, 2);
			
		//sleep(1);
	}

	return EXIT_SUCCESS;
}
