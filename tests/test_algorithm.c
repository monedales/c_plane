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

static int	test_all_simultaneous(void)
{
	int		entries[] = {1, 2, 3, 4, 5};
	int		exits[] = {10, 10, 10, 10, 10};
	t_room	room = make_room(5, entries, exits);

	return (max_simultaneous(&room) == 5);
}

static int	test_same_moment_entry_exit(void)
{
	int		entries[] = {1, 3, 5};
	int		exits[] = {3, 5, 7};
	t_room	room = make_room(3, entries, exits);

	return (max_simultaneous(&room) == 1);
}

static int	test_50_all_overlap(void)
{
	int		entries[50];
	int		exits[50];
	int		i;

	i = 0;
	while (i < 50)
	{
		entries[i] = i + 1;
		exits[i] = 951 + i;
		i++;
	}
	t_room	room = make_room(50, entries, exits);
	return (max_simultaneous(&room) == 50);
}

static int	test_50_sequential(void)
{
	int		entries[50];
	int		exits[50];
	int		i;

	i = 0;
	while (i < 50)
	{
		entries[i] = i * 2 + 1;
		exits[i] = i * 2 + 2;
		i++;
	}
	t_room	room = make_room(50, entries, exits);
	return (max_simultaneous(&room) == 1);
}

static int	test_100_all_overlap(void)
{
	int		entries[100];
	int		exits[100];
	int		i;

	i = 0;
	while (i < 100)
	{
		entries[i] = i + 1;
		exits[i] = 901 + i;
		i++;
	}
	t_room	room = make_room(100, entries, exits);
	return (max_simultaneous(&room) == 100);
}

static int	test_100_sequential(void)
{
	int		entries[100];
	int		exits[100];
	int		i;

	i = 0;
	while (i < 100)
	{
		entries[i] = i * 2 + 1;
		exits[i] = i * 2 + 2;
		i++;
	}
	t_room	room = make_room(100, entries, exits);
	return (max_simultaneous(&room) == 1);
}

static int	test_100_peak_in_middle(void)
{
	int		entries[100];
	int		exits[100];
	int		i;

	i = 0;
	while (i < 50)
	{
		entries[i] = 1;
		exits[i] = 500;
		i++;
	}
	while (i < 100)
	{
		entries[i] = 600;
		exits[i] = 1000;
		i++;
	}
	t_room	room = make_room(100, entries, exits);
	return (max_simultaneous(&room) == 50);
}

/* ************************************************************************** */
/*                          VALIDATION TESTS                                  */
/* ************************************************************************** */

static int	test_validation_valid(void)
{
	int		entries[] = {1, 5, 7};
	int		exits[] = {9, 13, 12};
	t_room	room = make_room(3, entries, exits);

	return (validate_input(&room) == NO_ERROR);
}

static int	test_validation_entry_after_exit(void)
{
	int		entries[] = {10, 5, 7};
	int		exits[] = {3, 13, 12};
	t_room	room = make_room(3, entries, exits);

	return (validate_input(&room) == ERR_ENTRY_AFTER_EXIT);
}

static int	test_validation_entry_equals_exit(void)
{
	int		entries[] = {1, 5, 5};
	int		exits[] = {9, 13, 5};
	t_room	room = make_room(3, entries, exits);

	return (validate_input(&room) == ERR_ENTRY_AFTER_EXIT);
}

static int	test_validation_null_room(void)
{
	return (validate_input(NULL) == ERR_READ_FAILURE);
}

/* ************************************************************************** */
/*                                  MAIN                                      */
/* ************************************************************************** */

int	main(void)
{
	printf("\n=== c_plane unit tests ===\n\n");

	printf("-- algorithm --\n");
	RUN_TEST(test_example_1);
	RUN_TEST(test_example_2);
	RUN_TEST(test_single_passenger);
	RUN_TEST(test_all_simultaneous);
	RUN_TEST(test_same_moment_entry_exit);

	printf("\n-- algorithm (large inputs) --\n");
	RUN_TEST(test_50_all_overlap);
	RUN_TEST(test_50_sequential);
	RUN_TEST(test_100_all_overlap);
	RUN_TEST(test_100_sequential);
	RUN_TEST(test_100_peak_in_middle);

	printf("\n-- validation --\n");
	RUN_TEST(test_validation_valid);
	RUN_TEST(test_validation_entry_after_exit);
	RUN_TEST(test_validation_entry_equals_exit);
	RUN_TEST(test_validation_null_room);

	printf("\n=== %d/%d tests passed ===\n\n", g_tests_passed, g_tests_run);

	return (g_tests_passed == g_tests_run ? 0 : 1);
}
