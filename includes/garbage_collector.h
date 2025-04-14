/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:36:49 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/09 15:32:43 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H
# include <stddef.h>
# include <stdlib.h>

# define MAX_GC_ALLOCS	2048

typedef struct s_allocation
{
	void	*pointer;
	size_t	size;
	int		used;
}	t_allocation;

typedef struct s_garbage_collector
{
	t_allocation	allocations[MAX_GC_ALLOCS];
	size_t			num_allocations;
	size_t			next_index;
}	t_garbage_collector;

void			gc_init(t_garbage_collector *garbage);
void			*gc_malloc(size_t size, t_garbage_collector *garbage);
int				gc_free(void *pointer, t_garbage_collector *garbage);
void			gc_clean(t_garbage_collector *garbage);
t_allocation	*gc_get(void *pointer, t_garbage_collector *garbage);
void			*gc_calloc(size_t count, size_t size, t_garbage_collector *gc);
t_allocation	*gc_next_free(t_garbage_collector *garbage);

#endif