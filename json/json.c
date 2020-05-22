#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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

struct node
{
	char data;	
	struct node *next;
	
};

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


void push(struct node **ptrhead, char data)
{
	struct node *new = (struct node *)malloc(sizeof(struct node));
	if (new == NULL) {
		printf("out of memory!\n");
		return;
	}
	new->data = data;
	new->next = (*ptrhead);

	(*ptrhead) = new;
}

char pop(struct node **ptrhead)
{
	char data;
	struct node *top;
	if (*ptrhead == NULL) {
		return 0;
	}

	top = *ptrhead;	
	data = top->data;	
	*ptrhead = top->next;
	free(top);
	
	return data;
}


/* 
   Returns 1 if character1 and character2 
   are matching left and right Parenthesis
*/
bool is_match(char character1, char character2) 
{ 
	if (character1 == '(' && character2 == ')') 
		return 1; 
	if (character1 == '{' && character2 == '}') 
		return 1; 
	if (character1 == '[' && character2 == ']') 
		return 1; 
	return 0; 
}

/*
  Return 1 if expression has balanced Parenthesis 
*/
bool parenthes_balanced(char exp[]) 
{ 
   int i = 0; 
 
   struct node *stack = NULL; 
  
   while (exp[i]) 
   { 
      if (exp[i] == '{' || exp[i] == '(' || exp[i] == '[') 
        push(&stack, exp[i]); 
  
      if (exp[i] == '}' || exp[i] == ')' || exp[i] == ']') 
      { 
         if (stack == NULL) 
           return 0;  
  
         else if ( !is_match(pop(&stack), exp[i]) ) 
           return 0; 
      } 
      i++; 
   } 
     
   if (stack == NULL) 
     return 1;
   return 0;
}


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
	chunk += step + 1;

		
	/* 
	 * value 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == ':' || *chunk == '"') chunk++;
		
	step = strcspn(chunk, "\",\0");
	lx->value = malloc(sizeof(char) * (step + 1));
	if (lx->value == NULL) 	return NULL;
	
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
	

	int i = 0;
	while (strlen(stream) > 0) {
	
		step = strcspn(stream, ",");
		chunk = malloc(sizeof(char) * (step + 1));
		memcpy(chunk, stream, step);
		chunk[step] = '\0';
		
		stream += step + 1;
		
		
		LEX *lx;
		lx = lexer(chunk);
		if (lx != NULL) {
			printf("%s", lx->key);
			if (lx->value != NULL) {
				printf(": %s", lx->value);
			}
			printf("\n");
		}
	}

	return;
}

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
	/*char *stream;
	stream = read_file("package-min.json");

	parse(stream);*/

	char exp[100] = "{()}[]"; 
	if (parenthes_balanced(exp)) 
		printf("Balanced \n"); 
	else
		printf("Not Balanced \n");   

	return 0;
}
