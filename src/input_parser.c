#include "../include/c_plane.h"

/* ************************************************************************** */
/*                          LINE READING                                      */
/* ************************************************************************** */

/**
 * @brief Reads a single line from stdin into the provided buffer.
 *
 * Wraps fgets with proper return handling. Considers EOF or read error as
 * a failure. The buffer must have capacity for size bytes and will hold
 * the line including the trailing newline if present.
 *
 * @param buffer Pre allocated buffer to store the read line.
 * @param size Maximum number of bytes to read, including the null terminator.
 * @return NO_ERROR on success, ERR_READ_FAILURE if fgets fails or EOF reached.
 */
static t_input_error	read_line(char *buffer, size_t size)
{
	if (!fgets(buffer, (int)size, stdin))
		return (ERR_READ_FAILURE);
	return (NO_ERROR);
}

/* ************************************************************************** */
/*                          INTEGER PARSING                                   */
/* ************************************************************************** */

/**
 * @brief Parses a single integer from a string using strtol.
 *
 * Uses strtol with base 10. Detects non numeric input via endptr equality
 * with the input pointer, and detects overflow via errno set to ERANGE.
 * The next pointer is updated to point to the first character that was
 * not part of the parsed integer.
 *
 * @param str Input string to parse.
 * @param result Output pointer for the parsed integer value.
 * @param next Output pointer for the position right after the parsed number.
 * @return NO_ERROR, ERR_NON_NUMERIC if nothing parsed, ERR_OUT_OF_RANGE on
 * overflow.
 */
static t_input_error	parse_single_int(const char *str, long *result,
		char **next)
{
	errno = 0;
	*result = strtol(str, next, 10);
	if (*next == str)
		return (ERR_NON_NUMERIC);
	if (errno == ERANGE)
		return (ERR_OUT_OF_RANGE);
	return (NO_ERROR);
}

/**
 * @brief Validates the trailing portion of a line after expected parsing.
 *
 * After reading the expected count of integers, the remaining characters
 * should be only whitespace. If a digit or sign character is found, it
 * means there are more numbers than expected (size mismatch). Any other
 * non whitespace character is treated as non numeric input.
 *
 * @param p Pointer to the first character after the last parsed integer.
 * @return NO_ERROR if only whitespace remains, ERR_SIZE_MISMATCH if extra
 * numbers found, ERR_NON_NUMERIC if invalid characters found.
 */
static t_input_error	check_trailing(const char *p)
{
	while (*p)
	{
		if (isspace((unsigned char)*p))
		{
			p++;
			continue ;
		}
		if (isdigit((unsigned char)*p) || *p == '-' || *p == '+')
			return (ERR_SIZE_MISMATCH);
		return (ERR_NON_NUMERIC);
	}
	return (NO_ERROR);
}

/* ************************************************************************** */
/*                          INTEGER LIST PARSING                              */
/* ************************************************************************** */

/**
 * @brief Parses a line containing a list of integers separated by whitespace.
 *
 * Allocates an array of the expected count and fills it with integers
 * parsed from the line. Each value must fall within [MIN_MOMENT, MAX_MOMENT].
 * After all expected values are parsed, the remaining characters in the line
 * must be only whitespace. On any error, the allocated array is freed and
 * the output pointer is set to NULL.
 *
 * @param line Input line containing the integer list.
 * @param array Output pointer for the allocated array of integers.
 * @param expected_count Number of integers expected in the line.
 * @return NO_ERROR, ERR_MEMORY_ALLOC, ERR_NON_NUMERIC, ERR_OUT_OF_RANGE,
 * ERR_INVALID_MOMENT or ERR_SIZE_MISMATCH.
 */
static t_input_error	parse_int_list(const char *line, int **array,
		int expected_count)
{
	const char		*p;
	char			*next;
	long			value;
	int				count;
	t_input_error	err;

	*array = calloc(expected_count, sizeof(int));
	if (!*array)
		return (ERR_MEMORY_ALLOC);
	p = line;
	count = 0;
	while (count < expected_count)
	{
		while (*p && isspace((unsigned char)*p))
			p++;
		if (*p == '\0')
		{
			free(*array);
			*array = NULL;
			return (ERR_SIZE_MISMATCH);
		}
		err = parse_single_int(p, &value, &next);
		if (err != NO_ERROR)
		{
			free(*array);
			*array = NULL;
			return (err);
		}
		if (value < MIN_MOMENT || value > MAX_MOMENT)
		{
			free(*array);
			*array = NULL;
			return (ERR_INVALID_MOMENT);
		}
		(*array)[count++] = (int)value;
		p = next;
	}
	err = check_trailing(p);
	if (err != NO_ERROR)
	{
		free(*array);
		*array = NULL;
	}
	return (err);
}

/* ************************************************************************** */
/*                          N PARSING                                         */
/* ************************************************************************** */

/**
 * @brief Parses the first line of the input to extract the value of N.
 *
 * Reads one integer from the line using parse_single_int, validates that
 * it falls within [MIN_PASSENGERS, MAX_PASSENGERS], and verifies the rest
 * of the line contains only whitespace (no extra data).
 *
 * @param line Input line containing the value of N.
 * @param n Output pointer to receive the parsed value of N.
 * @return NO_ERROR, ERR_NON_NUMERIC, ERR_OUT_OF_RANGE, ERR_INVALID_N or
 * ERR_SIZE_MISMATCH.
 */
static t_input_error	parse_n(const char *line, int *n)
{
	char			*next;
	long			value;
	t_input_error	err;

	err = parse_single_int(line, &value, &next);
	if (err != NO_ERROR)
		return (err);
	if (value < MIN_PASSENGERS || value > MAX_PASSENGERS)
		return (ERR_INVALID_N);
	err = check_trailing(next);
	if (err != NO_ERROR)
		return (err);
	*n = (int)value;
	return (NO_ERROR);
}

/* ************************************************************************** */
/*                          PUBLIC API                                        */
/* ************************************************************************** */

/**
 * @brief Reads and parses the full input from stdin into a t_room struct.
 *
 * Orchestrates the reading and parsing of the three input lines: the
 * passenger count N, the list of entry moments and the list of exit moments.
 * On any failure, ensures that any partially allocated memory is freed and
 * the room is left in a safe state.
 *
 * @param room Pointer to a pre allocated t_room struct to be filled.
 * @return NO_ERROR on success, or a specific t_input_error on failure.
 */
t_input_error	read_input(t_room *room)
{
	char			line[MAX_LINE_LEN];
	t_input_error	err;

	if (!room)
		return (ERR_READ_FAILURE);
	room->n = 0;
	room->entries = NULL;
	room->exits = NULL;
	err = read_line(line, sizeof(line));
	if (err != NO_ERROR)
		return (err);
	err = parse_n(line, &room->n);
	if (err != NO_ERROR)
		return (err);
	err = read_line(line, sizeof(line));
	if (err != NO_ERROR)
		return (err);
	err = parse_int_list(line, &room->entries, room->n);
	if (err != NO_ERROR)
		return (err);
	err = read_line(line, sizeof(line));
	if (err == NO_ERROR)
		err = parse_int_list(line, &room->exits, room->n);
	if (err != NO_ERROR)
		free_room(room);
	return (err);
}
