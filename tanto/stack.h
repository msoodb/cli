#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*
 * stack node
 */
struct stack_node
{
	char data;	
	struct stack_node *next;
};

/* 
 * stack push data 
*/
void stack_push(struct stack_node **stack, char data)
{
	struct stack_node *new = (struct stack_node *)malloc(sizeof(struct stack_node));
	if (new == NULL) return;
	
	new->data = data;
	new->next = (*stack);

	(*stack) = new;
}

/* 
 * stack pop data 
*/
char stack_pop(struct stack_node **stack)
{
	char data;
	struct stack_node *top;
	if (*stack == NULL) return 0;

	top = *stack;	
	data = top->data;	
	*stack = top->next;
	free(top);
	
	return data;
}

#endif // _STACK_H_
