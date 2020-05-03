/*
 * Copyright (c) 2020-2020 kci
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
#include <getopt.h>
#include <syslog.h>


#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>


#define PROGRAM_NAME "kci"
#define PORT_NO 5955
#define BUFFER_SIZE 1024

static char *conf_file_name = NULL;
static char *log_file_name = NULL;
static char *server_ip = NULL;

int create_socket();
int connect_socket(int socket_desc);
int send_sockect(int socket_desc, char* send_buffer, short buffer_len);
int receive_socket(int socket_desc, char* recieve_buffer, short buffer_len);
void print_help(void);
char* resolve_host(const char* host);

int main(int argc, char *argv[])
{
	int socket_desc = 0, n = 0;
	char send_buffer[BUFFER_SIZE] = {0};
	char recieve_buffer[BUFFER_SIZE] = {0};	
	struct sockaddr_in server;
	char *host;
	/*int show_time, show_date;*/
	
	
	/*show_time = show_date = 0;*/

	static struct option long_options[] = {
		{"time",      no_argument,       0, 't'},
		{"date",      no_argument,       0, 'd'},
		{"conf_file", required_argument, 0, 'c'},		
		{"log_file",  required_argument, 0, 'l'},
		{"help",      no_argument,       0, 'h'},
	        {NULL, 0, 0, 0}
	};
	int value, option_index = 0;

	while ((value = getopt_long(argc, argv, "c:l:tdh", long_options, &option_index)) != -1) {
		switch (value) {
		case 't':
			//show_time = 1;
			break;
		case 'd':
			//show_date = 1;
			break;		
		case 'c':
			conf_file_name = strdup(optarg);
			break;
		case 'l':
			log_file_name = strdup(optarg);
			break;		
		case 'h':
			print_help();
			return EXIT_SUCCESS;
		case '?':
			print_help();
			return EXIT_FAILURE;
		default:
			break;
		}
	}
	
	if(argc == 1){
		printf("%s\n", "klc: You must specify a host to connect to. QUITTING");
		return 1;
	}
	host = argv[--argc];

	if ((server_ip = resolve_host(host)) == NULL) {
		printf("%s%s%s\n", "Could not resolve hostname \"", server_ip,
		       "\": Name or service not known. QUITTING.");
		return 1;
	}
	
	memset(recieve_buffer, '0',sizeof(recieve_buffer));
	memset(&server, '0', sizeof(server));
	
	if ((socket_desc = create_socket()) == -1){                                               
                printf("%s\n", "Faield! create socket");                          
                return 1;                                                     
        }                                                                                     
        printf("%s\n", "[Info]: Socket created");
	
	if( connect_socket(socket_desc)){
		printf("%s\n", "Error: Connect Failed");
		return -1;
	}
	printf("%s\n", "[Info]: Socket connected");

	
	strcat(send_buffer, "show_time:1");
	if (send_sockect(socket_desc, send_buffer, strlen(send_buffer)) < 0) {
		printf("%s\n", "Error: send failed");
		return 1;	
	}
	printf("%s\n", "[Info]: Send succeeded");
	

	while ((n = receive_socket(socket_desc, recieve_buffer, strlen(recieve_buffer))) > 0) {
		recieve_buffer[n] = 0;
		printf("%s\n", "[Info]: Recieve succeeded");
		printf("%s", recieve_buffer);
	}

	close(socket_desc);
	shutdown(socket_desc, 0);
	shutdown(socket_desc, 1);
	shutdown(socket_desc, 2);

	return 0;
}

int create_socket()
{
	return socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
}  

int connect_socket(int socket_desc)
{	
        struct sockaddr_in remote = {0};
	
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = inet_addr(server_ip);
	remote.sin_port = htons(PORT_NO);
	
	/*if(inet_pton(AF_INET, server_ip, &remote.sin_addr)<=0){
		return -1;
		} */

	return connect(socket_desc, (struct sockaddr *)&remote, sizeof(remote));
}

int send_sockect(int socket_desc, char* send_buffer, short buffer_len)
{
    struct timeval tv;
    
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;
    
    if (setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0){
        return -1;
    }

    return send(socket_desc, send_buffer, buffer_len, 0);
}

int receive_socket(int socket_desc, char* recieve_buffer, short buffer_len)
{   
    struct timeval tv;

    tv.tv_sec = 20;
    tv.tv_usec = 0;
        
    if (setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0){
        return -1;
    }

    return recv(socket_desc, recieve_buffer, buffer_len, 0);
}

void print_help(void)
{
	printf("\n Usage: %s [OPTIONS] destination\n\n", PROGRAM_NAME);
	printf("  Options:\n");
	printf("   -h --help                 Print this help\n");
	printf("   -c --conf_file filename   Read configuration from the file\n");
	printf("   -t --time                 Show time\n");
	printf("   -d --date                 show date\n");
	printf("   -l --log_file  filename   Write logs to the file\n");
	
	printf("\n");
}

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
