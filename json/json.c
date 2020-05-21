#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char *json_tv =
"{\n\
\t\"name\": \"Awesome 4K\",\n\
\t\"version\": \"0.1.0\",\n\
\t\"private\": true,\n\
\t\"resolutions\": [\n\
\t\t{\n\
\t\t\t\"width\": 1280,\n\
\t\t\t\"height\": 720\n\
\t\t}, \n\
\t\t{\n\
\t\t\t\"width\": 1920,\n\
\t\t\t\"height\": 1080\n\
\t\t}, \n\
\t\t{\n\
\t\t\t\"width\": 3840,\n\
\t\t\t\"height\": 2160\n\
\t\t}\n\
\t],\n\
\t\"eslintConfig\": {\n\
\t\t\"root\": true,\n\
\t\t\"env\": { \n\
\t\t\t\"node\": true \n\
\t\t}, \n\
\t\t\"extends\": [ \n\
\t\t\t\"plugin:vue/essential\", \n\
\t\t\t\"eslint:recommended\"\n\
\t\t],\n\
\t\t\"parserOptions\": {\n\
\t\t\t\"parser\": \"babel-eslint\"\n\
\t\t},\n\
\t\t\"rules\": {}\n\
\t}\n\
}";

static const char *json =
"{\n\t\"name\": \"Awesome 4K\",\n\t\"version\": \"0.1.0\",\n\t\"private\": true\n}";


typedef struct _lex
{
	int type; // 0: object, 1: filed, 2: array 
	char *key;
	char *value;
} LEX;

typedef struct _json
{
	struct _json *prev;
	struct _json *child;
	struct _json *next;	
} JSON;

LEX *lexer(char *chunk)
{
	LEX *lx;
	size_t step;
	
	lx = malloc(sizeof(LEX) * 1);
	if (lx == NULL) {
		return NULL;
	}

	
	/* type */	
	lx->type = 0;

	/* 
	 * key 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == '"') chunk++;
	if (strlen(chunk) == 0) return NULL;
	

	step = strcspn(chunk, "\"\0");

	lx->key = malloc(sizeof(char) * (step + 1));	
	if (lx->key == NULL) return NULL;
	
	memcpy(lx->key, chunk, step);
	lx->key[step] = '\0';

	if (strlen(chunk) == 1){
		lx->type = 0;
		lx->value = NULL;
		return lx;
	}
	chunk += step + 1;

		
	/* 
	 * value 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == ':' || *chunk == '"') chunk++;
	
	
	step = strcspn(chunk, "\",\0");
	lx->value = malloc(sizeof(char) * (step + 1));
	if (lx->value == NULL) {
		return NULL;
	}
	memcpy(lx->value, chunk, step);
	lx->value[step] = '\0';

	return lx;
}

void parse(const char *stream)
{	
	char *chunk;
	size_t step;

	chunk = NULL;
	step = 0;
	


	while (strlen(stream) > 0) {		
		step = strcspn(stream, ",\n");
		chunk = malloc(sizeof(char) * (step + 1));
		memcpy(chunk, stream, step);
		chunk[step] = '\0';
		stream += step + 1;
		
		LEX *lx;
		lx = lexer(chunk);
		if (lx != NULL) {
			printf("%s", lx->key);
			if (lx->value != NULL) {
				printf(":%s", lx->value);
			}
			printf("\n");
		}		
	}

	return;
}

/*void parse(char *file)
{	
	FILE *fp;
	char *chunk;
	size_t chunk_len;
	
	fp = fopen(file, "r");
	if (fp == NULL) return;
	
	chunk = NULL;
	
	while (getline(&chunk, &chunk_len, fp) != -1) {

		LEX *lx;
		lx = lexer(chunk);
		if (lx == NULL) return;

		printf("%s", lx->key);
		printf("%s\n", lx->value);			
		
		chunk = NULL;
        }
	fclose(fp);
	
	return;
	}*/

char *read_file(const char *file)
{
	FILE *fp;
	long f_size;
	char *stream;
	
	fp = NULL;
	f_size = 0;
	
      	fp = fopen(file, "r");
	if (fp == NULL) return NULL;

	fseek(fp, 0L, SEEK_END);
        f_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);                               

	stream = (char *)malloc(sizeof(char) * (f_size + 1));             
        fread(stream, sizeof(char), f_size, fp);
	fclose(fp);

	stream[f_size] = '\0';	
	return stream;
}

int main()
{
	char *stream;
	stream = read_file("simple.json");

	printf("%s", stream);

	parse(stream);

	return 0;
}
