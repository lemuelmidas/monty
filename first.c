#include "main.h"

void push(stack_t **stack, unsigned int line_number);
void pall(stack_t **stack, unsigned int line_number);
void pint(stack_t **stack, unsigned int line_number);
void pop(stack_t **stack, unsigned int line_number);
void swap(stack_t **stack, unsigned int line_number);

/**
 * push - a function to push a value to a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number of a Monty bytecodes file.
 */
void push(stack_t **stack, unsigned int line_number)
{
	stack_t *mem, *new;
	int a;

	new = malloc(sizeof(stack_t));
	if (new == NULL)
	{
		set_op_tok_error(malloc_error());
		return;
	}

	if (op_toks[1] == NULL)
	{
		set_op_tok_error(no_int_error(line_number));
		return;
	}

	for (a = 0; op_toks[1][a]; a++)
	{
		if (op_toks[1][a] == '-' && a == 0)
			continue;
		if (op_toks[1][a] < '0' || op_toks[1][a] > '9')
		{
			set_op_tok_error(no_int_error(line_number));
			return;
		}
	}
	new->n = atoi(op_toks[1]);

	if (check_mode(*stack) == STACK) /* STACK mode insert at front */
	{
		mem  = (*stack)->next;
		new->prev = *stack;
		new->next = mem;
		if (mem)
			mem->prev = new;
		(*stack)->next = new;
	}
	else /* QUEUE mode insert at end */
	{
		mem = *stack;
		while (mem->next)
			mem = mem->next;
		new->prev = mem;
		new->next = NULL;
		mem->next = new;
	}
}

/**
 * pall - a function to prints the values of a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number of a Monty bytecodes file.
 */
void pall(stack_t **stack, unsigned int line_number)
{
	stack_t *mem = (*stack)->next;

	while (mem)
	{
		printf("%d\n", mem->n);
		mem = mem->next;
	}
	(void)line_number;
}

/**
 * pint - a function to print the top value of a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number of a Monty bytecodes file.
 */
void pint(stack_t **stack, unsigned int line_number)
{
	if ((*stack)->next == NULL)
	{
		set_op_tok_error(pint_error(line_number));
		return;
	}

	printf("%d\n", (*stack)->next->n);
}


/**
 * pop - a function to remove the top value element of a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number of a Monty bytecodes file.
 */
void pop(stack_t **stack, unsigned int line_number)
{
	stack_t *next = NULL;

	if ((*stack)->next == NULL)
	{
		set_op_tok_error(pop_error(line_number));
		return;
	}

	next = (*stack)->next->next;
	free((*stack)->next);
	if (next)
		next->prev = *stack;
	(*stack)->next = next;
}

/**
 * swap - a function to swap the top two value elements of a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number of a Monty bytecodes file.
 */
void swap(stack_t **stack, unsigned int line_number)
{
	stack_t *mem;

	if ((*stack)->next == NULL || (*stack)->next->next == NULL)
	{
		set_op_tok_error(short_stack_error(line_number, "swap"));
		return;
	}

	mem = (*stack)->next->next;
	(*stack)->next->next = mem->next;
	(*stack)->next->prev = mem;
	if (mem->next)
		mem->next->prev = (*stack)->next;
	mem->next = (*stack)->next;
	mem->prev = *stack;
	(*stack)->next = mem;
}
