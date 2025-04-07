/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_managing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:20:37 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/02 18:32:16 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>

void	gc_init(t_garbage_collector *garbage)
{
	ft_bzero(garbage, sizeof(t_garbage_collector));
}

void	gc_clean(t_garbage_collector *garbage)
{
	t_allocation	*alloc;
	size_t			iterations;
	
	if (!garbage)
		return ;
	iterations = 0;
	while (iterations < MAX_GC_ALLOCS && garbage->num_allocations > 0)
	{
		alloc = &garbage->allocations[iterations];
		if (alloc->used == 1 && alloc->pointer)
		{
			garbage->num_allocations--;
			free(alloc->pointer);
			alloc->pointer = NULL;
		}
		alloc->size = 0;
		alloc->used = 0;
		iterations++;
	}
	garbage->num_allocations = 0;
	garbage->next_index = 0;
}

t_allocation	*gc_get(void *pointer, t_garbage_collector *garbage)
{
    t_allocation *alloc;
    size_t iterations;
    size_t index;

    if (!garbage || garbage->num_allocations <= 0)
        return (NULL);
    index = garbage->next_index;
    iterations = 0;
    
    while (iterations < MAX_GC_ALLOCS)
    {
        alloc = &garbage->allocations[index];
        if (alloc->used == 1 && alloc->pointer == pointer)
            return (alloc);
        
        iterations++;
        index = (index + MAX_GC_ALLOCS - 1) % MAX_GC_ALLOCS;
    }
    return (NULL);
}

t_allocation	*gc_next_free(t_garbage_collector *garbage)
{
	int				iterations;
	t_allocation	*alloc;

	if (!garbage)
		return (NULL);
	iterations = 0;
	while (iterations < MAX_GC_ALLOCS)
	{
		alloc = &garbage->allocations[garbage->next_index];
		if (!alloc->used)
			return (alloc);
		garbage->next_index = (garbage->next_index + 1) % MAX_GC_ALLOCS;
		iterations++;
	}
	garbage->next_index = 0;
    garbage->num_allocations = MAX_GC_ALLOCS;
    return (NULL);
}
