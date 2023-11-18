#include "main.h"
#include <string.h>

void free_tokens(void);
unsigned int token_arr_len(void);
int is_empty_line(char *line, char *delims);
void (*get_op_func(char *opcode))(stack_t**, unsigned int);
int run_monty(FILE *script_fd);

/**
 * free_tokens - to free the global op_toks array of strings.
 */
void free_tokens(void)
{
	size_t a = 0;

	if (op_toks == NULL)
		return;

	for (a = 0; op_toks[a]; a++)
		free(op_toks[a]);

	free(op_toks);
}

/**
 * token_arr_len - function to get the length of current op_toks.
 *
 * Return: Length of current op_toks (as int).
 */
unsigned int token_arr_len(void)
{
	unsigned int toks_size = 0;

	while (op_toks[toks_size])
		toks_size++;
	return (toks_size);
}

/**
 * is_empty_line - a function to check if a line read from getline only contains delimiters.
 * @line: A pointer to the line.
 * @delims: A string of delimiter characters.
 *
 * Return: If the line only contains delimiters - 1.
 *         Otherwise - 0.
 */
int is_empty_line(char *line, char *delims)
{
	int a, b;

	for (a = 0; line[a]; a++)
	{
		for (b = 0; delims[b]; b++)
		{
			if (line[b] == delims[b])
				break;
		}
		if (delims[b] == '\0')
			return (0);
	}

	return (1);
}

/**
 * get_op_func - a function to matche an opcode with its corresponding function.
 * @opcode: The opcode to match.
 *
 * Return: The function's pointer.
 */
void (*get_op_func(char *opcode))(stack_t**, unsigned int)
{
	instruction_t op_funcs[] = {
		{"push", push},
		{"pall", pall},
		{"pint", pint},
		{"pop", pop},
		{"swap", swap},
		{"add", add},
		{"nop", nop},
		{"sub", sub},
		{"div", monty_div},
		{"mul", mul},
		{"mod", mod},
		{"pchar", pchar},
		{"pstr", pstr},
		{"rotl", rotl},
		{"rotr", rotr},
		{"stack", stack},
		{"queue", queue},
		{NULL, NULL}
	};
	int a;

	for (a = 0; op_funcs[a].opcode; a++)
	{
		if (strcmp(opcode, op_funcs[a].opcode) == 0)
			return (op_funcs[a].f);
	}

	return (NULL);
}

/**
 * run_monty - Primary function to execute a Monty bytecodes script.
 * @script_fd: File descriptor for an open Monty bytecodes script.
 *
 * Return: if successful, 0, else -1.
 */
int run_monty(FILE *script_fd)
{
	stack_t *stack = NULL;
	char *line = NULL;
	size_t len = 0, exit_status = EXIT_SUCCESS;
	unsigned int line_number = 0, prev_tok_len = 0;
	void (*op_func)(stack_t**, unsigned int);

	if (init_stack(&stack) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	while (getline(&line, &len, script_fd) != -1)
	{
		line_number++;
		op_toks = strtow(line, DELIMS);
		if (op_toks == NULL)
		{
			if (is_empty_line(line, DELIMS))
				continue;
			free_stack(&stack);
			return (malloc_error());
		}
		else if (op_toks[0][0] == '#') /* comment line */
		{
			free_tokens();
			continue;
		}
		op_func = get_op_func(op_toks[0]);
		if (op_func == NULL)
		{
			free_stack(&stack);
			exit_status = unknown_op_error(op_toks[0], line_number);
			free_tokens();
			break;
		}
		prev_tok_len = token_arr_len();
		op_func(&stack, line_number);
		if (token_arr_len() != prev_tok_len)
		{
			if (op_toks && op_toks[prev_tok_len])
				exit_status = atoi(op_toks[prev_tok_len]);
			else
				exit_status = EXIT_FAILURE;
			free_tokens();
			break;
		}
		free_tokens();
	}
	free_stack(&stack);

	if (line && *line == 0)
	{
		free(line);
		return (malloc_error());
	}

	free(line);
	return (exit_status);
}
