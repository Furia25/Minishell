/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_managing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 19:12:44 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/11 19:53:48 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool		hashmap_init_basics(t_hashmap *map)
{
	map->size = 1 << HASHMAP_POWER;
	map->table = ft_calloc(map->size + 1, sizeof(t_hash_entry));
	if (!map->table)
		return (false);
	map->count = 0;
	map->charge_factor = HASHMAP_POWER;
	return (true);
}

t_hashmap	*hashmap_new(int power, double chargefactor)
{
	t_hashmap	*result;

	result = ft_calloc(1, sizeof(t_hashmap));
	if (!result)
		return (NULL);
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

void	hashmap_free(t_hashmap *map, void (*del)(void *))
{
	hashmap_free_content(map, del);
	free(map);
}

void	hashmap_free_content(t_hashmap *map, void (*del)(void *))
{
	size_t	index;

	if (del)
	{
		index = 0;
		while (index < map->size)
		{
			if (map->table[index].status == OCCUPIED)
				del(map->table[index].value);
			index++;
		}
	}
	if (map->table)
		free(map->table);
}
