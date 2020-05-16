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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


/*
 * Creation: FILE *fp
 * Opening:  fopen
 * Reading:  getline
 * Writing:  fprintf
 * Closing:  fclose
 */

char* concat(const char *s1, const char *s2)
{
	char *result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

int file_exists(const char *file)
{
	struct stat stats;
	stat(file, &stats);
	if (stats.st_mode & R_OK) return 1;
	return 0;
}

void write_chunk(char *file, char *chunk)
{
	FILE *fp;
	fp = fopen(file, "a");

	if(fp == NULL) return;
	
	fprintf(fp, "%s", chunk);
	
	fclose(fp);
}

void read_chunk(char *file)
{	
	FILE *fp;
	
	char *chunk;
	size_t chunk_len;	
	
	fp = fopen(file, "r");
	if (fp == NULL) return;
		
	chunk = NULL;

	int i = 0;
	while (getline(&chunk, &chunk_len, fp) != -1) {

		int length = snprintf(NULL, 0, "%3d", i);
		char* str = malloc(length + 1);
		snprintf(str, length + 1, "%03d", i);

		write_chunk(concat("directory/file_", concat(str, ".txt")), chunk);

		i++;

	}

	fclose(fp);
	
	if (chunk) free(chunk);
}

char *read_file(const char *file)
{
	FILE *fp;
	long f_size;
	char *buf;
	
	fp = NULL;
	f_size = 0;
	
      	fp = fopen(file, "r");
	if (fp == NULL) return NULL;

	fseek(fp, 0L, SEEK_END);
        f_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);                               

	buf = (char *)malloc(sizeof(char) * (f_size + 1));             
        fread(buf, sizeof(char), f_size, fp);
	fclose(fp);

	buf[f_size] = '\0';	
	return buf;
}

int main(int argc, char *argv[])
{
	struct stat st = {0};
	if (stat("directory", &st) == -1) {
		mkdir("directory", 0700);
	}

	read_chunk("file.c");
	
	return 0;
}
