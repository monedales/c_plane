#include <stdbool.h>
#include <stdlib.h>
#include "../include/c_plane.h"

/* ************************************************************************** */
/*                          EVENT STRUCT (INTERNAL)                           */
/* ************************************************************************** */

typedef struct s_event
{
	int		moment;
	bool	is_entry;
}	t_event;

/* ************************************************************************** */
/*                          COMPARATOR FOR QSORT                              */
/* ************************************************************************** */

/*
** compare_events - ordena eventos por momento. Em caso de empate,
** saídas (is_entry=false=0) vêm antes de entradas (is_entry=true=1).
** Isso garante a regra: se alguém entra no mesmo momento que outro sai,
** o que saiu não conta mais na contagem simultânea.
*/
static int	compare_events(const void *a, const void *b)
{
	const t_event	*ea = (const t_event *)a;
	const t_event	*eb = (const t_event *)b;

	if (ea->moment != eb->moment)
		return (ea->moment - eb->moment);
	return (ea->is_entry - eb->is_entry);
}

/* ************************************************************************** */
/*                          POPULATE EVENTS ARRAY                             */
/* ************************************************************************** */

/*
** populate_events - transforma as listas E e S do room em um array
** unificado de eventos, onde cada passageiro gera 2 eventos:
** uma entrada (+1) e uma saída (-1).
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

/*
** sweep_events - percorre os eventos já ordenados acumulando a
** contagem atual e guardando o pico máximo de passageiros simultâneos.
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

/*
** max_simultaneous - função principal que orquestra o algoritmo
** Sweep Line para encontrar o número máximo de passageiros
** simultaneamente na sala de espera.
**
** Retorna -1 em caso de falha de alocação.
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
