/*
 * Copyright (c) 2020-2020 cmock
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

/* basic headers */
#include <stdio.h>            /* io     */
#include <stdlib.h>           /* memory */
#include <getopt.h>           /* cli    */
#include <string.h>           /* strcpy */ 
#include <stdbool.h>          /* bool   */ 

/* advanced headers */
#include <unistd.h>          /* sleep    */
#include <signal.h>          /* SIGINT   */  
#include <stdarg.h>          /* va_start */

/* The official name of this program (e.g., no 'g' prefix).  */
#define _PROGRAM_NAME       "cli"
#define _PROGRAM_DESC       "command line arguments and options management"
#define _PROGRAM_AUTHORS    "msoodb (Masoud Bolhassani)"
#define _PROGRAM_VERSION    "0.0.1"
#define _PROGRAM_URL        "https://msoodb.org/cli"

#define EXIT_NONE -1

/* True if the --verbose or -v in option.  */
static bool f_verbose = false;

#define COLOR_OPT      1000
#define OUTPUT_OPT     1100
#define VERSION_OPT    1600

static struct option const long_options[] = {		
	{"config",      required_argument,  0,            'c'},
	{"verbose",     no_argument,        0,            'v'},
	{"color",       optional_argument,  0,      COLOR_OPT},
	{"output",      required_argument,  0,     OUTPUT_OPT},
	{"help",        no_argument,        0,            'h'},
	{"version",     no_argument,        0,    VERSION_OPT},
	{NULL, 0, 0, 0}
};

int vbprintf(const char *fmt, ...)
{
	va_list ap;
	int res = 0;

	if (f_verbose) {
		va_start(ap, fmt);
		res = vprintf(fmt, ap);
		va_end(ap);
	}
	return res;
}

void help(int status)
{
	printf("%s %s (%s)\n", _PROGRAM_NAME, _PROGRAM_VERSION, _PROGRAM_URL);
	printf("Usage: %s [OPTIONS] SOURCE PORT\n", _PROGRAM_NAME);
	printf("%s\n", _PROGRAM_DESC);
	printf("\nOptions:\n");
	printf("   -c, --config file    Read config from the config file.\n");
	printf("   -v, --verbose        Explain what is being done.\n");
	printf("       --color          Haracteristic of visual perception described.\n");
	printf("       --output=TYPE    Output data type, text, json, table.\n");
	printf("   -h, --help           Display this help and exit.\n");
	printf("       --version        Output version information and exit.\n");	
	printf("%s %s%s\n", "See the", _PROGRAM_NAME,
	       "(1) manpage for full options, descriptions and usage examples.");
	printf("\n");

	if (status != EXIT_NONE) exit(status);
}

void version(void)
{
	printf("%s %s (%s)\n", _PROGRAM_NAME, _PROGRAM_VERSION, _PROGRAM_URL);
	printf("\n");
	printf("%s\n", "Copyright (C) 2019 Free Software Foundation, Inc.");
	printf("%s\n", "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.");
	printf("%s\n", "This is free software: you are free to change and redistribute it.");
	printf("%s\n", "There is NO WARRANTY, to the extent permitted by law.");
	printf("\n");
	printf("%s %s.\n", "Written by", _PROGRAM_AUTHORS);
}

void sig_handler(int signo)
{
	if (signo == SIGINT){
		printf("\n%s\n", "Received SIGINT");
		exit(EXIT_FAILURE);
	}
}

void terminate()
{
	return;
}


int main(int argc, char *argv[])
{
	/* program variables */
	int port;
	char *source;
	char *config;
	char *color;
	char *output;
	
	port = -1;
	source = NULL;
	config = NULL;
	color = NULL;
	output = NULL;
		
	/* sign handler and register terminate function at exit */
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		printf("\n%s\n","can't catch SIGINT");
	atexit(terminate);

	/* cli menu section */
	int value, option_index;
	
	if (argc == 1) help(EXIT_FAILURE);
		
	while (1) {
		value = option_index = 0;
		
		value = getopt_long(argc, argv, "-:hvc:", long_options, &option_index);
		if (value == -1) break;

		switch (value) {
		case 'c':
			if (optarg) config = strdup(optarg);
			break;
		case 'v':
			f_verbose = true;
			break;
		case COLOR_OPT:
			if (optarg) color = strdup(optarg);
			break;
		case OUTPUT_OPT:
			if (optarg) output = strdup(optarg);
			break;
		case 'h':
			help(EXIT_SUCCESS);
			break;
		case VERSION_OPT:
			version();
			return EXIT_SUCCESS;
		case 1:
			printf("%s\n", optarg);
			if (atoi(optarg) > 1000 && atoi(optarg) < 65000) {
				port = atoi(optarg); 
			}
			break;		
		case '?':
			help(EXIT_FAILURE);
			break;
		case ':':
			printf("%s: %s %c\n", _PROGRAM_NAME, "missing arg for", optopt);
			printf("%s\n", "Try 'cmock --help' for more information.");
			exit(EXIT_FAILURE);
			break;
		}
	}

	if (port == -1) {
		printf("%s: %s\n", _PROGRAM_NAME, "missing PORT number");
		printf("%s%s %s\n", "Try '", _PROGRAM_NAME, "--help' for more information.");
		exit(EXIT_FAILURE);
	}
	vbprintf("%s %d\n", "Port is set to", port);

	if (config != NULL) {
		vbprintf("%s %s\n", "Config file is", config);
	}

	if (color != NULL) {
		vbprintf("%s %s\n", "Color is", color);
	}

	if (output != NULL) {
		vbprintf("%s %s\n", "Output type is", output);
	}
	

	int i = 0;
	while (i++ < 5) {
		sleep(1);
	}

	return 0;
}
