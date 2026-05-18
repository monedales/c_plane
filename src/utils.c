#include "../include/c_plane.h"

/**
 * @brief Handle and display error messages.
 *
 * This function takes an error code from the t_input_error enum and prints
 * the corresponding error message to standard error. It uses a static array
 * of error messages indexed by the error code. The function always returns
 * EXIT_ERR to indicate that an error occurred.
 *
 * @param error The error code from the t_input_error enum.
 * @return Always returns EXIT_ERR (1) to indicate an error condition.
 */
int	handle_error(t_input_error error)
{
	static const char	*messages[] = {
		NULL,
		"Error: failed to read input from stdin\n",
		"Error: N must be an integer between 1 and 100\n",
		"Error: moment values must be integers between 1 and 1000\n",
		"Error: list size does not match the number of passengers\n",
		"Error: a passenger entry must occur before its exit\n",
		"Error: input contains non numeric characters. Use plain numbers separated by spaces.\n",
		"Error: numeric value is out of allowed range\n",
		"Error: memory allocation failed\n"
	};
	size_t				total;

	total = sizeof(messages) / sizeof(messages[0]);
	if (error > 0 && (size_t)error < total)
		fprintf(stderr, "%s", messages[error]);
	return (EXIT_ERR);
}

/**
 * @brief Frees all dynamically allocated memory inside a t_room struct.
 *
 * Releases the entries and exits arrays if they were allocated, and sets the
 * pointers back to NULL to prevent accidental reuse. Safe to call with a
 * NULL pointer or with arrays that were never allocated.
 *
 * @param room Pointer to the t_room struct to be cleaned up.
 */
void	free_room(t_room *room)
{
	if (!room)
		return ;
	if (room->entries)
	{
		free(room->entries);
		room->entries = NULL;
	}
	if (room->exits)
	{
		free(room->exits);
		room->exits = NULL;
	}
	room->n = 0;
}
