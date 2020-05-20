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
#include <unistd.h>
#include <string.h>
#include <netdb.h>
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
  |  connect()   |
  |______________|
         |
   ______V_______         
  |              |        
  |    write()   |<---    
  |______________|    |   
         |            |   
   ______V_______     |   
  |              |    |     
  |    read()    |----      
  |______________|          
         |
   ______V_______ 
  |              |
  |    close()   |
  |______________|

Ref: 
 The Definitive Guide to Linux Network Programming.
 Keir Davis, John W. Turner, and Nathan Yocom.
*/


#define PORT_NO 4545
#define BUFFER_SIZE 1024


static char *server_ip = NULL;

char* resolve_host(const char *host)
{
	char *ip;
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
		
	if ((he = gethostbyname(host)) == NULL){
		return NULL;
	}
	
	/* Cast the h_addr_list to in_addr, 
	 *since h_addr_list also has the ip 
	 *address in long format only
	 */
	addr_list = (struct in_addr **) he->h_addr_list;
	
	ip = malloc(sizeof(char) * 100);
	for(i = 0; addr_list[i] != NULL; i++){
		strcpy(ip , inet_ntoa(*addr_list[i]) );
	}
	
	printf("%s resolved to : %s\n" , host , ip);
	return ip;	
}

int main(int argc, char *argv[])
{
	int socket_desc = 0, n = 0;
	char send_buffer[BUFFER_SIZE] = {0};
	char recieve_buffer[BUFFER_SIZE] = {0};	
	struct sockaddr_in server;
	struct timeval tv;
	char *host;
			
	host = argv[--argc];
	memset(recieve_buffer, '0',sizeof(recieve_buffer));
	memset(&server, '0', sizeof(server));
    
	tv.tv_sec = 20;
	tv.tv_usec = 0;

	if ((server_ip = resolve_host(host)) == NULL) {
		printf("%s%s%s\n", "Could not resolve hostname \"", server_ip,
		       "\": Name or service not known. QUITTING.");
		return EXIT_FAILURE;
	}
	
	/* socket */
	if ((socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1){                                               
                printf("%s\n", "Faield! create socket");                          
                return 1;                                                     
        }                                                                                     
        printf("%s\n", "Socket");


	/* connect */		
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(server_ip);
	server.sin_port = htons(PORT_NO);
	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server))){
		printf("%s\n", "Error: Connect Failed");
		return -1;
	}
	printf("%s\n", "Connect");


	while (1) {
		/* write */
		if (setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0){
			return -1;
		}

		strcat(send_buffer, "show_time:1");
		if(send(socket_desc, send_buffer, strlen(send_buffer), 0) < 0 ){
			printf("%s\n", "Error: send failed");
			return 1;	
		}
		printf("%s\n", "Write");


		sleep(1);
		
		/* read */
		if (setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0){
			return -1;
		}

		if ((n = recv(socket_desc, recieve_buffer, strlen(recieve_buffer), 0)) < 0){
			printf("%s\n", "Error: recieve failed");
			return 1;
		}
		recieve_buffer[n] = 0;
		printf("%s\n", "Read");
		//printf("%s", recieve_buffer);		
	}
	
	/* close */
	close(socket_desc);

	return 0;
}
