#include "../include/c_plane.h"

/* ************************************************************************** */
/*             EVENT STRUCT (INTERNAL FOR INFORMATION HIDING)                 */
/* ************************************************************************** */

typedef struct s_event
{
	int		moment;
	bool	is_entry;
}	t_event;

/* ************************************************************************** */
/*                          COMPARATOR FOR QSORT                              */
/* ************************************************************************** */

/**
 * @brief Comparison function used by qsort to order events.
 *
 * Orders events primarily by their moment in ascending order. When two
 * events share the same moment, exits (is_entry false, value 0) are
 * placed before entries (is_entry true, value 1). This ordering enforces
 * the rule that a passenger entering at the same moment another is leaving
 * does not share the room with that passenger in the simultaneous count.
 *
 * @param a Generic pointer to the first event.
 * @param b Generic pointer to the second event.
 * @return Negative if a comes first, positive if b comes first, zero if equal.
 */
static int	compare_events(const void *a, const void *b)
{
	const t_event	*ea;
	const t_event	*eb;
	int				moment_diff;
	int				entry_order;

	ea = (const t_event *)a;
	eb = (const t_event *)b;
	moment_diff = ea->moment - eb->moment;
	entry_order = ea->is_entry - eb->is_entry;
	if (moment_diff != 0)
		return (moment_diff);
	return (entry_order);
}

/* ************************************************************************** */
/*                          POPULATE EVENTS ARRAY                             */
/* ************************************************************************** */

/**
 * @brief Populates an array of events from the entries and exits lists.
 *
 * For each passenger in the room, generates two events in the events array:
 * one entry event marked with is_entry true and one exit event marked with
 * is_entry false. The resulting array has exactly room->n * 2 events.
 *
 * @param events Pre allocated array with capacity for room->n * 2 events.
 * @param room Pointer to the t_room struct containing entries and exits.
 */
static void	populate_events(t_event *events, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->n)
	{
		events[i * 2].moment = room->entries[i];
		events[i * 2].is_entry = true;
		events[i * 2 + 1].moment = room->exits[i];
		events[i * 2 + 1].is_entry = false;
		i++;
	}
}

/* ************************************************************************** */
/*                          SWEEP LINE CORE                                   */
/* ************************************************************************** */

/**
 * @brief Walks through ordered events counting simultaneous passengers.
 *
 * Iterates through the already sorted events array, incrementing the
 * current count on entries and decrementing on exits. The maximum value
 * reached by the current count during the traversal represents the peak
 * of simultaneous passengers in the room.
 *
 * @param events Array of events already sorted by compare_events.
 * @param total Total number of events in the array (room->n * 2).
 * @return The maximum number of passengers simultaneously in the room.
 */
static int	sweep_events(t_event *events, int total)
{
	int	current;
	int	max;
	int	i;

	current = 0;
	max = 0;
	i = 0;
	while (i < total)
	{
		if (events[i].is_entry)
			current++;
		else
			current--;
		if (current > max)
			max = current;
		i++;
	}
	return (max);
}

/* ************************************************************************** */
/*                          PUBLIC API                                        */
/* ************************************************************************** */

/**
 * @brief Computes the maximum number of simultaneous passengers in the room.
 *
 * Orchestrates the Sweep Line algorithm in three stages: builds a unified
 * array of entry and exit events, sorts that array by moment with the
 * appropriate tie breaking rule, and walks through the sorted events
 * tracking the peak simultaneous count.
 *
 * @param room Pointer to the t_room struct with n, entries and exits filled.
 * @return The maximum number of simultaneous passengers, or -1 on allocation
 * failure.
 */
int	max_simultaneous(t_room *room)
{
	t_event	*events;
	int		total;
	int		result;

	total = room->n * 2;
	events = malloc(sizeof(t_event) * total);
	if (!events)
		return (-1);
	populate_events(events, room);
	qsort(events, total, sizeof(t_event), compare_events);
	result = sweep_events(events, total);
	free(events);
	return (result);
}
