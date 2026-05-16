#ifndef C_PLANE_H
# define C_PLANE_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/* ************************************************************************** */
/*                                CONSTANTS                                   */
/* ************************************************************************** */

# define MIN_PASSENGERS 1
# define MAX_PASSENGERS 100
# define MIN_MOMENT     1
# define MAX_MOMENT     1000

# define EXIT_OK        0
# define EXIT_ERR       1

/* ************************************************************************** */
/*                                STRUCTS                                     */
/* ************************************************************************** */

typedef struct s_room
{
	int	n;
	int	*entries;
	int	*exits;
}	t_room;

/* ************************************************************************** */
/*                              FUNCTION PROTOTYPES                           */
/* ************************************************************************** */

/* input.c */
int		read_input(t_room *room);

/* validation.c */
int		validate_input(t_room *room);

/* algorithm.c */
int		max_simultaneous(t_room *room);

/* utils */
void	free_room(t_room *room);
void	print_error(const char *msg);

#endif
