#include "../include/c_plane.h"

/**
 * @brief Program entry point that orchestrates the full pipeline.
 *
 * Coordinates the three stages of the program: reading and parsing the
 * input from stdin, validating the business rules over the parsed data
 * and computing the maximum number of simultaneous passengers using
 * the Sweep Line algorithm.
 *
 * Uses the goto cleanup pattern to centralize memory release. Any error
 * along the pipeline triggers a jump to the cleanup label, which ensures
 * that the t_room struct is always properly freed before the program
 * exits, regardless of which stage failed.
 *
 * The output is the maximum simultaneous count followed by a newline,
 * as specified by the challenge statement. On error, an error message
 * is printed to stderr and the program returns EXIT_ERR.
 *
 * @return EXIT_OK on success, EXIT_ERR on any failure.
 */
int	main(void)
{
	t_room			room;
	t_input_error	err;
	int				result;
	int				exit_code;

	exit_code = EXIT_OK;
	err = read_input(&room);
	if (err != NO_ERROR)
	{
		exit_code = handle_error(err);
		goto cleanup;
	}
	err = validate_input(&room);
	if (err != NO_ERROR)
	{
		exit_code = handle_error(err);
		goto cleanup;
	}
	result = max_simultaneous(&room);
	if (result < 0)
	{
		exit_code = handle_error(ERR_MEMORY_ALLOC);
		goto cleanup;
	}
	printf("%d\n", result);
cleanup:
	free_room(&room);
	return (exit_code);
}
