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
"{\n\
\t\"name\": \"Awesome 4K\",\n\
\t\"version\": \"0.1.0\",\n\
\t\"private\": true\n\
}";


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


	/* key */
	while(isspace((unsigned char)*chunk) || *chunk == '"') chunk++;
	step = strcspn(chunk, "\"\r\n");
	lx->key = malloc(sizeof(char) * (step + 1));	
	if (lx->key == NULL) {
		return NULL;
	}	
	memcpy(lx->key, chunk, step);
	lx->key[step] = '\0';
	chunk += step + 1;

	/* value */
	while(isspace((unsigned char)*chunk) || *chunk == ':' || *chunk == '"') chunk++;
	step = strcspn(chunk, "\",\r\n");
	lx->value = malloc(sizeof(char) * (step + 1));
	if (lx->value == NULL) {
		return NULL;
	}
	memcpy(lx->value, chunk, step);
	lx->value[step] = '\0';

	return lx;
}

int main()
{
	printf("%s\n", json);

	//char *line = "\t\"name\":\"Awesome 4K\",\n";
	char *line = "{\n";
	printf("%s\n", line);

	LEX *lx;
	lx = lexer(line);
	if (lx == NULL) {
		return 0;
	}

	printf("%s:", lx->key);
	printf("%s\n", lx->value);			


	return 0;
}
