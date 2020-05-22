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
	struct __json__ *child;
	struct __json__ *next;
	char *key;
	char *value;
	int type;
} JSON;

#define IS_EMPTY_LIST(lsit)  ((list) == NULL)



JSON *json_init_node(char *key, char *value, int type)
{
	JSON *new = (JSON*) malloc(sizeof(JSON) * 1);
	if (new == NULL) return NULL;
	
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
}

JSON *json_pop(JSON **list)
{
	if (*list == NULL) return NULL;
	
	JSON *current = *list;

	*list = (*list)->next;
	
	return current;
}

JSON *json_find(JSON *list, char *key)
{	
	while (list != NULL) {
		if (strcmp(list->key, key) == 0) return list;
		list = list->next;
	}
	return NULL;
}

void json_print(JSON *list)
{
	while (list != NULL) {
		printf("%s:", list->key);
		printf("%s\n", list->value);
		list = list->next;
	}	
}

void json_print_addr(JSON *list)
{
	while (list != NULL) {

		printf("\t%s\n", "  ----------");
		printf("%8d", list);
		printf("%s", " | ");
		printf("%8d %s\n", list->next, "|");
		printf("\t%s\n\n", "  ----------");		

		list = list->next;
	}	
}

JSON *json_lexer(char *chunk)
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

	node = json_init_node(key, value, type);

	return node;
}

JSON *json_parse(const char *stream)
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
				
		node = json_lexer(chunk);
		json_push(&list, node);
	}

	return list;
}

char *json_read_file(const char *file)
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

void json_write_file(char *file, JSON *list)
{
	FILE *fp;
	fp = fopen(file, "w");
	if(fp == NULL) return;

	bool comma;
	comma = false;
	
	fprintf(fp, "%c\n", '{');
	while (list != NULL) {
		if (comma) fprintf(fp, "%c\n", ',');
		fprintf(fp, "\t");
		fprintf(fp, "\"%s\":", list->key);
		fprintf(fp, " \"%s\"", list->value);
		comma = true;
		list = list->next;
	}
	fprintf(fp, "\n%c\n", '}');

	fclose(fp);
}

bool is_match(char character1, char character2) 
{ 
	if (character1 == '(' && character2 == ')') return 1; 
	else if (character1 == '{' && character2 == '}') return 1; 
	else if (character1 == '[' && character2 == ']') return 1; 
	return 0; 
}

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

int main()
{
	char *stream;
	stream = json_read_file("package-min.json");

	//parse(stream);

	/*char *exp = "{()}[fghfgh sdgf(gfh{d}fgh)sdfg[]]"; 
	if (parenthes_balanced(stream)) 
		printf("Balanced \n"); 
	else
		printf("Not Balanced \n");   
	*/

	JSON *list = NULL;
	
	list = json_parse(stream);
	json_print(list);

	JSON *node;
	node = json_init_node("name", "masoud", 1);
	json_push(&list, node);


	/* find */
	/*JSON *nodef = NULL;
	nodef = json_find(list, "@vue/cli-service");
	if (nodef != NULL) {
		printf("%s\n", nodef->value);
		}*/
	
	//json_print_addr(list);


	json_write_file("test.json", list);

	return 0;
}
