/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_managing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:20:37 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/14 01:01:58 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>
#include <stdint.h>

bool	gc_init(t_minishell *data)
{
	return (hashmap_init_basics(&data->gc, free));
}

void	gc_clean(t_minishell *data)
{
	if (data->gc.table)
		hashmap_free_content(&data->gc);
}

void	gc_add(void *ptr, t_minishell *data)
{
	unsigned long	hashed;

	if (ptr == NULL)
		return ;
	hashed = hash_ptr(ptr);
	if (hashmap_insert(hashed, ptr, &data->gc) == 0)
	{
		free(ptr);
		malloc_error(data);
	}
}
