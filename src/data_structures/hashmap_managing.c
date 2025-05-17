/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_managing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 19:12:44 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/17 14:29:26 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	hashmap_init_basics(t_hashmap *map, void (*del)(void *))
{
	map->del = del;
	map->size = 1 << HASHMAP_POWER;
	map->table = ft_calloc(map->size + 1, sizeof(t_hash_entry));
	if (!map->table)
		return (false);
	map->count = 0;
	map->charge_factor = HASHMAP_CHARGEFACTOR;
	return (true);
}

t_hashmap	*hashmap_new(int power, double chargefactor, void (*del)(void *))
{
	t_hashmap	*result;

	result = ft_calloc(1, sizeof(t_hashmap));
	if (!result)
		return (NULL);
	result->del = del;
	result->size = 1 << power;
	result->table = ft_calloc(result->size + 1, sizeof(t_hash_entry));
	if (!result->table)
	{
		free(result);
		return (NULL);
	}
	result->count = 0;
	result->charge_factor = chargefactor;
	return (result);
}

void	hashmap_free(t_hashmap *map)
{
	if (!map)
		return ;
	hashmap_free_content(map);
	free(map);
}

void	hashmap_free_content(t_hashmap *map)
{
	size_t	index;

	if (map->table == NULL)
		return ;
	index = 0;
	while (index < map->size)
	{
		if (map->table[index].status == OCCUPIED)
			map->del(map->table[index].value);
		index++;
	}
	if (map->table)
		free(map->table);
}
