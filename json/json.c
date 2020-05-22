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

#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct __json__
{
	struct __json__ *prev;	
	struct __json__ *child;
	struct __json__ *next;
	char *key;
	char *value;
	int type;
} JSON;

typedef struct _key_value
{
	char *key;
	char *value;
} KEY_VALUE;



#define IS_EMPTY_LIST(lsit)  ((list) == NULL)

JSON *init_json_node(char *key, char *value, int type)
{
	JSON *new = (JSON*) malloc(sizeof(JSON) * 1);
	if (new == NULL) return NULL;
	
	new->prev = NULL;
	new->child = NULL;
	new->next = NULL;

	new->key = key;
	new->value = value;
	new->type = type;

	return new;
}

void json_push(JSON **list, JSON *node)
{
	JSON *current = *list;
	*list = node;

	node->next = current;
	if (current) {
		current->prev = node;
	}
}

JSON *json_pop(JSON **list)
{
	if (*list == NULL) return NULL;
	
	JSON *current = *list;

	*list = (*list)->next;
	if (*list) {
		(*list)->prev = NULL;
	}
	
	return current;
}

/*void print(JSON *list)
{
	int node_number = 0;
	while (list != NULL) {

		printf("\t\t%s\n", " ---------------------");
		printf("%8d", list);
		printf("\t%s %8d", "|", list->prev);
		printf("%s", " | ");
		printf("%8d %s\n", list->next, "|");
		printf("\t\t%s\n\n", " ---------------------");		

		list = list->next;
	}	
	}*/

void print(JSON *list)
{
	int node_number = 0;
	while (list != NULL) {
		printf("%s:", list->key);
		printf("%s\n", list->value);
		list = list->next;
	}	
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
 
	struct stack_node *stack = NULL; 
  
	while (exp[i]){ 
		if (exp[i] == '{' || exp[i] == '(' || exp[i] == '[') 
			stack_push(&stack, exp[i]); 
  
		if (exp[i] == '}' || exp[i] == ')' || exp[i] == ']'){ 
			if (!is_match(stack_pop(&stack), exp[i])) return 0;
		} 
		i++; 
	} 
     
	if (stack == NULL) return 1;
   
	return 0;
}

JSON *lexer(char *chunk)
{
	JSON *node;
	size_t step;

	char *key;
	char *value;
	int type;
	
	node = NULL;
	
	/* 
	 * key 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == '"') chunk++;
	if (strlen(chunk) == 0) return NULL;
	

	step = strcspn(chunk, "\"\0");

	key = malloc(sizeof(char) * (step + 1));	
	if (key == NULL) return NULL;
	
	memcpy(key, chunk, step);
	key[step] = '\0';
	chunk += step + 1;

	/*
	 * type
	 */
	type = 0;
		
	/* 
	 * value 
	 */
	while(isspace((unsigned char)*chunk) || *chunk == ':' || *chunk == '"') chunk++;
		
	step = strcspn(chunk, "\",\0");
	value = malloc(sizeof(char) * (step + 1));
	if (value == NULL) return NULL;
	
	memcpy(value, chunk, step);
	value[step] = '\0';

	node = init_json_node(key, value, type);

	return node;
}

JSON *parse(const char *stream)
{
	char *chunk;
	size_t step;
	JSON *list = NULL;
	JSON *node = NULL;
	
	chunk = NULL;
	step = 0;
	

	int i = 0;
	while (strlen(stream) > 0) {
	
		step = strcspn(stream, ",");
		chunk = malloc(sizeof(char) * (step + 1));
		memcpy(chunk, stream, step);
		chunk[step] = '\0';
		
		stream += step + 1;
				
		node = lexer(chunk);
		json_push(&list, node);
	}

	return list;
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
	stream = read_file("package-min.json");

	//parse(stream);

	/*char *exp = "{()}[fghfgh sdgf(gfh{d}fgh)sdfg[]]"; 
	if (parenthes_balanced(stream)) 
		printf("Balanced \n"); 
	else
		printf("Not Balanced \n");   
	*/

	JSON *list = NULL;
	
	list = parse(stream);
	print(list);

	return 0;
}
