/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_methods.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:18:50 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/23 14:29:03 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdint.h>

static void	swap(t_hash_entry *a, t_hash_entry *b)
{
	t_hash_entry	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief Resizes the hashmap's internal table.
 *
 * Allocates a new table, rehashes existing entries,
 * and uses linear probing
 * for collisions. The table is expanded if the load
 * factor threshold is exceeded.
 *
 * @param new_size The new table size.
 * @param map A pointer to the hashmap structure.
 * @return 1 if successful, 0 otherwise.
 */
int	hashmap_resize(size_t new_size, t_hashmap *map)
{
	t_hash_entry	*new_table;
	t_hash_entry	*old_table;
	size_t			old_size;
	size_t			index;

	if (new_size == 0 || new_size > SIZE_MAX / 2)
		return (0);
	new_table = ft_calloc(new_size, sizeof(t_hash_entry));
	if (!new_table)
		return (0);
	old_table = map->table;
	old_size = map->size;
	map->table = new_table;
	map->size = new_size;
	map->count = 0;
	index = 0;
	while (index < old_size)
	{
		if (old_table[index].status == OCCUPIED)
			hashmap_insert(old_table[index].key, old_table[index].value, map);
		index++;
	}
	free(old_table);
	return (1);
}

static void	prcs(unsigned long key, size_t i,
				t_hash_entry last, t_hashmap *map)
{
	void	*old_value;

	while (1)
	{
		if (map->table[i].status == EMPTY || map->table[i].status == TOMBSTONE)
		{
			if (map->table[i].status == TOMBSTONE && map->table[i].value)
				map->del(map->table[i].value);
			map->table[i] = last;
			map->count++;
			return ;
		}
		if (map->table[i].status == OCCUPIED && map->table[i].key == key)
		{
			old_value = map->table[i].value;
			map->del(old_value);
			map->table[i] = last;
			return ;
		}
		if (last.probe_distance > map->table[i].probe_distance)
			swap(&last, &map->table[i]);
		last.probe_distance++;
		i = (i + 1) & (map->size - 1);
	}
}

/**
 * @brief Inserts a key-value pair into the hashmap.
 *
 * If the key already exists, updates the value.
 * Resizes the hashmap if the load factor
 * exceeds the threshold. Linear probing resolves
 * collisions with robin hood opti.
 *
 * @param key The key to insert.
 * @param value The associated value.
 * @param map A pointer to the hashmap structure.
 * @return 1 if successful, 0 on error.
 */
int	hashmap_insert(unsigned long key, void *value, t_hashmap *map)
{
	size_t			pos;
	t_hash_entry	last;

	if (!map)
		return (0);
	if (!value)
		return (0);
	if ((double)(map->count + 1) / map->size >= map->charge_factor)
		if (!hashmap_resize(map->size << 1, map))
			return (0);
	pos = key & (map->size - 1);
	last = (t_hash_entry){.key = key, .value = value, \
		.status = OCCUPIED, .probe_distance = 0};
	prcs(key, pos, last, map);
	return (1);
}

/**
 * @brief Searches for a key in the hashmap.
 *
 * Returns the value associated with the key,
 * or NULL if the key is not found.
 * Linear probing resolves collisions with robin hood opti.
 *
 * @param key The key to search for.
 * @param map A pointer to the hashmap structure.
 * @return The associated value, or NULL if not found.
 */
t_hash_entry	*hashmap_search(unsigned long key, t_hashmap *map)
{
	size_t	pos;
	size_t	dist;

	dist = 0;
	pos = key & (map->size - 1);
	while (map->table[pos].status != EMPTY)
	{
		if (map->table[pos].status != TOMBSTONE)
			if (map->table[pos].key == key)
				return (&map->table[pos]);
		if (dist > map->table[pos].probe_distance)
			return (NULL);
		pos = (pos + 1) & (map->size - 1);
		dist++;
	}
	return (NULL);
}
