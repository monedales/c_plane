#ifndef C_PLANE_H
# define C_PLANE_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <ctype.h>
# include <errno.h>

/* ************************************************************************** */
/*                                CONSTANTS                                   */
/* ************************************************************************** */

# define MIN_PASSENGERS 1
# define MAX_PASSENGERS 100
# define MIN_MOMENT     1
# define MAX_MOMENT     1000

# define MAX_LINE_LEN   1024

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

typedef enum e_input_error
{
	NO_ERROR = 0,
	ERR_READ_FAILURE,
	ERR_INVALID_N,
	ERR_INVALID_MOMENT,
	ERR_SIZE_MISMATCH,
	ERR_ENTRY_AFTER_EXIT,
	ERR_NON_NUMERIC,
	ERR_OUT_OF_RANGE,
	ERR_MEMORY_ALLOC
}				t_input_error;

/* ************************************************************************** */
/*                              FUNCTION PROTOTYPES                           */
/* ************************************************************************** */

/* input_parser.c */
t_input_error	read_input(t_room *room);

/* validation.c */
t_input_error	validate_input(t_room *room);

/* algorithm.c */
int				max_simultaneous(t_room *room);

/* utils.c */
int				handle_error(t_input_error error);
void			free_room(t_room *room);

/* lib/ft_string.c */
char			*ft_substr(const char *s, size_t start, size_t len);
char			**ft_split(const char *s, char c);

/* lib/ft_memory.c */
void			ft_free_arr(char **arr, size_t until);

#endif
