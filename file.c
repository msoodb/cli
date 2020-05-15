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


/*
 * @file
 * @chunk
 * Append chunk at the end of file.
 */
void write_chunk(char *file, char *chunk)
{
	FILE *fp;
	fp = fopen(file, "a");

	if(fp == NULL) return;
	
	fprintf(fp, "%s", chunk);
	
	fclose(fp);
}

/*
 * @path
 * Read file line by line and exec some function on each chunk
 */
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
		write_chunk("README.md~", chunk);
	}

	fclose(fp);
	
	if (chunk) free(chunk);
}

/*
 * @path
 * Read whole file and return char * buffer
 */
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
	char *buff;
	//buff = read_file("README.md");

	read_chunk("README.md");


	if (buff != NULL) free(buff);
	
	return 0;
}
