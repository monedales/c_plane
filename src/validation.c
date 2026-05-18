#include "../include/c_plane.h"

/* ************************************************************************** */
/*                          PUBLIC API                                        */
/* ************************************************************************** */

/**
 * @brief Validates business rules over the input data already parsed.
 *
 * Iterates through the entries and exits arrays comparing each passenger's
 * own entry and exit moments. For a passenger to be valid, their entry
 * moment must be strictly less than their exit moment. If any passenger
 * has entry greater or equal to their exit, the input is rejected.
 *
 * Format level validations such as N range, moment range, list size and
 * numeric format are already performed during input parsing, so this
 * function focuses purely on business semantics.
 *
 * Note: equality between an entry of one passenger and an exit of another
 * passenger (E[i] == S[j] for i != j) is allowed and handled by the
 * algorithm itself through the ordering rule on tied moments.
 *
 * @param room Pointer to a t_room already filled by read_input.
 * @return NO_ERROR if all passengers have valid entry/exit pairs,
 * ERR_ENTRY_AFTER_EXIT if any passenger violates the rule, ERR_READ_FAILURE
 * if the room or its arrays are not properly initialized.
 */
t_input_error	validate_input(t_room *room)
{
	int	i;

	if (!room || !room->entries || !room->exits)
		return (ERR_READ_FAILURE);
	i = 0;
	while (i < room->n)
	{
		if (room->entries[i] >= room->exits[i])
			return (ERR_ENTRY_AFTER_EXIT);
		i++;
	}
	return (NO_ERROR);
}
