#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/c_plane.h"

/* ************************************************************************** */
/*                              TEST HELPERS                                  */
/* ************************************************************************** */

static int	g_tests_run = 0;
static int	g_tests_passed = 0;

#define RUN_TEST(test) do { \
	printf("Running %s... ", #test); \
	g_tests_run++; \
	if (test()) { \
		printf("PASS\n"); \
		g_tests_passed++; \
	} else { \
		printf("FAIL\n"); \
	} \
} while (0)

static t_room	make_room(int n, int *entries, int *exits)
{
	t_room	room;

	room.n = n;
	room.entries = entries;
	room.exits = exits;
	return (room);
}

/* ************************************************************************** */
/*                          ALGORITHM TESTS                                   */
/* ************************************************************************** */

static int	test_example_1(void)
{
	int		entries[] = {1, 5, 7};
	int		exits[] = {9, 13, 12};
	t_room	room = make_room(3, entries, exits);

	return (max_simultaneous(&room) == 3);
}

static int	test_example_2(void)
{
	int		entries[] = {1, 4, 8, 10};
	int		exits[] = {3, 8, 10, 17};
	t_room	room = make_room(4, entries, exits);

	return (max_simultaneous(&room) == 1);
}

static int	test_single_passenger(void)
{
	int		entries[] = {5};
	int		exits[] = {10};
	t_room	room = make_room(1, entries, exits);

	return (max_simultaneous(&room) == 1);
}

/* ************************************************************************** */
/*                                  MAIN                                      */
/* ************************************************************************** */

int	main(void)
{
	printf("\n=== c_plane unit tests ===\n\n");

	RUN_TEST(test_example_1);
	RUN_TEST(test_example_2);
	RUN_TEST(test_single_passenger);

	printf("\n=== %d/%d tests passed ===\n\n", g_tests_passed, g_tests_run);

	return (g_tests_passed == g_tests_run ? 0 : 1);
}
