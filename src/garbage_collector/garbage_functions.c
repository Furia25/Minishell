/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:58:23 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/02 18:33:00 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*gc_calloc(size_t el_count, size_t el_size, t_garbage_collector *gc)
{
	void	*temp;
	size_t	size;

	if (el_size == 0 || el_count == 0)
		return (malloc(0));
	if (el_count > __SIZE_MAX__ / el_size \
		|| el_size > __SIZE_MAX__ / el_count)
		return (NULL);
	size = el_count * el_size;
	temp = gc_malloc(size, gc);
	if (!temp)
		return (NULL);
	ft_memset(temp, 0, size);
	return (temp);
}

void	*gc_malloc(size_t size, t_garbage_collector *garbage)
{
	void			*result;
	t_allocation	*alloc;

	if (!garbage || size == 0)
		return (NULL);
	if (garbage->num_allocations >= MAX_GC_ALLOCS)
	{
		ft_putstr_fd("GC : Maximum numbers of allocations reached\n", 2);
		gc_clean(garbage);
		return (NULL);
	}
	alloc = gc_next_free(garbage);
	result = malloc(size);
	if (!result)
	{
		gc_clean(garbage);
		return (NULL);
	}
	garbage->num_allocations++;
	alloc->pointer = result;
	alloc->size = size;
	alloc->used = 1;
	return (result);
}

int	gc_free(void *pointer, t_garbage_collector *garbage)
{
	t_allocation	*alloc;

	if (!garbage)
		return (0);
	alloc = gc_get(pointer, garbage);
	if (!alloc)
	{
		if (DEBUG == 1)
			ft_putstr_fd("GC : Tried to free something inexistant\n", 2);
		return (0);
	}
	alloc->size = 0;
	alloc->used = 0;
	if (alloc->pointer)
		free(alloc->pointer);
	alloc->pointer = NULL;
	garbage->num_allocations -= 1;
	return (1);
}
