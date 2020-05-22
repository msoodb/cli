#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


/*
 * stack node
 */
struct node
{
	char data;	
	struct node *next;
};

typedef struct _json
{
	struct _json *prev;
	struct _json *child;
	struct _json *next;	
} JSON;


typedef struct _key_value
{
	char *key;
	char *value;
} KEY_VALUE;


/* 
 * stack push data 
*/
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

/* 
 * stack pop data 
*/
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
	if (character1 == '(' && character2 == ')') return 1; 
	else if (character1 == '{' && character2 == '}') return 1; 
	else if (character1 == '[' && character2 == ']') return 1; 
	return 0; 
}

/*
  Return 1 if expression has balanced Parenthesis 
*/
bool parenthes_balanced(char *exp) 
{ 
	int i = 0; 
 
	struct node *stack = NULL; 
  
	while (exp[i]){ 
		if (exp[i] == '{' || exp[i] == '(' || exp[i] == '[') 
			push(&stack, exp[i]); 
  
		if (exp[i] == '}' || exp[i] == ')' || exp[i] == ']'){ 
			if (!is_match(pop(&stack), exp[i])) return 0;
		} 
		i++; 
	} 
     
	if (stack == NULL) return 1;
   
	return 0;
}


KEY_VALUE *key_valuer(char *chunk)
{
	KEY_VALUE *kval;
	size_t step;
	
	kval = malloc(sizeof(KEY_VALUE) * 1);
	if (kval == NULL) {
		return NULL;
	}

	
	/* 
	 * key 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == '"') chunk++;
	if (strlen(chunk) == 0) return NULL;
	

	step = strcspn(chunk, "\"\0");

	kval->key = malloc(sizeof(char) * (step + 1));	
	if (kval->key == NULL) return NULL;
	
	memcpy(kval->key, chunk, step);
	kval->key[step] = '\0';
	chunk += step + 1;

		
	/* 
	 * value 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == ':' || *chunk == '"') chunk++;
		
	step = strcspn(chunk, "\",\0");
	kval->value = malloc(sizeof(char) * (step + 1));
	if (kval->value == NULL) return NULL;
	
	memcpy(kval->value, chunk, step);
	kval->value[step] = '\0';

	return kval;
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
		
		
		KEY_VALUE *kval;
		kval = key_valuer(chunk);
		if (kval != NULL) {
			printf("%s", kval->key);
			if (kval->value != NULL) {
				printf(": %s", kval->value);
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
	char *stream;
	stream = read_file("package.json");

	//parse(stream);

	char *exp = "{()}[fghfgh sdgf(gfh{d}fgh)sdfg[]]"; 
	if (parenthes_balanced(stream)) 
		printf("Balanced \n"); 
	else
		printf("Not Balanced \n");   

	return 0;
}
