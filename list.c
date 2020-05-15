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
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") should use only
 * in clist.h for manipulating node and whole lists, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

/*
 * struct node - doubly linked list node structure
 * typedef NODE
 */
typedef struct node{
	struct node *prev;
	struct node *next;
} NODE;

#define IS_EMPTY_LIST(lsit)  ((list) == NULL)

/*
 *  __init_node - create new node of NODE.
 *
 * The result is 1 if list is empty.
 * The result is 0 if list is not empty.
 */
NODE *__init_node()
{
	NODE *new = (NODE*)malloc(sizeof(NODE));
	if (new == NULL) return NULL;
	
	new->prev = NULL;
	new->next = NULL;

	return new;
}

/*
 * push - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
void push(NODE **list)
{
	NODE *new = __init_node();
	NODE *current = *list;
	
	*list = new;	
	new->next = current;
	if (current) {
		current->prev = new;
	}
}

/*
 * append - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
void append(NODE **list)
{
	if (*list == NULL) {
		push(list);
		goto out;
	}
	
	NODE *current = *list;
	NODE *itr = current;
	NODE *new = __init_node();

	while (itr != NULL) {
		current = itr;
		itr = itr->next;
	}
	
	current->next = new;
	new->prev = current;	
out:
	return;
}

/*
 * pop - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
void pop(NODE **list)
{
	if (*list == NULL) return;
	
	NODE *current = *list;

	*list = (*list)->next;
	if (*list) {
		(*list)->prev = NULL;
	}
	
	free(current);

	return;
}

/*
 * top - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
int top(NODE *list)
{
	if (list != NULL) return 1;
	
	return 0;
}

/*
 * reverse - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
void reverse(NODE **list)
{
	NODE *prev = NULL;
	NODE *current = *list;
	NODE *itr = current;

	while (itr != NULL) {
		current = itr;
		itr = itr->next;		
		current->next = prev;
		current->prev = itr;
		prev = current;		
	}
	
	*list = current;
}

/*
 * delete - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
int delete (NODE **list, int location)
{
	NODE *current = *list;
	NODE *itr = current;

	int i = 0;
	while (itr != NULL) {

		current = itr;
		itr = itr->next;
		
		if (i++ == (location - 1)) {
			printf("%d\n", location);
			current->next = itr->next;			
			if (current->next) {
				(current->next)->prev = current;
			}
			free(itr);			
			goto out_success;
		}		
	}

out_failure:
	return 0;

out_success:
	return 1;
}

/*
 * erase - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
void erase(NODE **list)
{
	NODE *current = *list;
	
	while (*list) {
		current = *list;
		*list = (*list)->next;
		free(current);
	}
}

/*
 * lenght - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
int lenght(NODE *list)
{
	int lenght = 0;
	NODE *current = list;
	
	while (current != NULL) {
		++lenght;
		current = current->next;
	}
	
	return lenght;
}

/*
 * print - push a new node into list
 * @list: pointer to (address of) pointer to node structure.
 *
 * Initializes the list if it is NULL.
 */
void print(NODE *list)
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
}

int main(int argc, char *argv[])
{
	NODE *list = NULL;

	if (IS_EMPTY_LIST(list)) {
		printf("%s\n", "list is empty");			
	}
	
	append(&list);	
	append(&list);

	int i;
	for (i = 0; i < 4; ++i) {				
		push(&list);		
	}
	
	print(list);
	
	return EXIT_SUCCESS;
}
