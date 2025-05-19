/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:10:14 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/19 18:55:28 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hashmap_iterate(void (*f) (unsigned long key, void *), t_hashmap *map)
{
	size_t			index;
	int				actual_count;
	t_hash_entry	entry;

	index = 0;
	actual_count = 0;
	while (index < map->size && actual_count < map->count)
	{
		entry = map->table[index];
		if (entry.status == OCCUPIED)
		{
			f(entry.key, entry.value);
			actual_count++;
		}
		index++;
	}
}

void	hashmap_remove(unsigned long key, t_hashmap *map)
{
	t_hash_entry	*entry;

	entry = hashmap_search(key, map);
	if (entry != NULL)
	{
		entry->status = TOMBSTONE;
		if (entry->value)
		{
			map->del(entry->value);
			entry->value = NULL;
			map->count -= 1;
		}
	}
}

unsigned long	hash(char *str)
{
	unsigned long	hash;
	unsigned long	c;
	int				i;

	hash = 0x811c9dc5;
	i = 0;
	while (str[i] != '\0')
	{
		c = (unsigned long)str[i];
		hash ^= c;
		hash *= 0x5bd1e995;
		hash ^= hash >> 15;
		i++;
	}
	return (hash);
}
