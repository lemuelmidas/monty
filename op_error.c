#include "main.h"

/**
 * op_error - a function to set last element of op_toks to be an error code.
 * @error_code: Integer to store as a string in op_toks.
 */
void op_error(int error_code)
{
	int toks_size = 0, a = 0;
	char *exit_str = NULL;
	char **new_toks = NULL;

	toks_size = token_arr_len();
	new_toks = malloc(sizeof(char *) * (toks_size + 2));
	if (!op_toks)
	{
		malloc_error();
		return;
	}
	while (a < toks_size)
	{
		new_toks[a] = op_toks[a];
		a++;
	}
	exit_str = get_int(error_code);
	if (!exit_str)
	{
		free(new_toks);
		malloc_error();
		return;
	}
	new_toks[a++] = exit_str;
	new_toks[a] = NULL;
	free(op_toks);
	op_toks = new_toks;
}
