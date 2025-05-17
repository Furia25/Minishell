/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_managing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:20:37 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/17 19:11:17 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>
#include <stdint.h>

bool	gc_init(t_minishell *data)
{
	return (hashmap_init_basics(&data->gc, free));
}

void	gc_full_clean(t_minishell *data)
{
	if (data->gc.table)
		hashmap_free_content(&data->gc);
}

void	gc_clean(t_minishell *data)
{
	if (data->gc.table)
	{
		if (hashmap_resize(data->gc.size, &data->gc) == 0)
			malloc_error(data);
	}
}

void	gc_add(void *ptr, t_minishell *data)
{
	unsigned long	hashed;

	if (ptr == NULL)
		return ;
	hashed = hash_ptr(ptr);
	if (hashmap_search(hashed, &data->gc) != NULL)
		return ;
	if (hashmap_insert(hashed, ptr, &data->gc) == 0)
	{
		free(ptr);
		malloc_error(data);
	}
}

void	*check_malloc(void *content, t_minishell *data)
{
	if (content == NULL)
	{
		malloc_error(data);
		return (NULL);
	}
	gc_add(content, data);
	return (content);
}
