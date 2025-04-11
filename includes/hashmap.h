/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:20:54 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/11 19:36:19 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_H
# define HASHMAP_H
# include <stddef.h>
# define HASHMAP_POWER	10
# define HASHMAP_CHARGEFACTOR	0.7

typedef enum e_hash_status
{
	EMPTY,
	TOMBSTONE,
	OCCUPIED
}			t_hash_status;

typedef struct s_hash_entry
{
	unsigned long	key;
	void			*value;
	t_hash_status	status;
	size_t			probe_distance;
}					t_hash_entry;

typedef struct s_hashmap
{
	t_hash_entry	*table;
	size_t			size;
	int				count;
	double			charge_factor;
}			t_hashmap;

// hashmap_managing.c

t_hashmap		*hashmap_new(int power, double chargefactor);
bool			hashmap_init_basics(t_hashmap *map);
void			hashmap_free(t_hashmap *map, void (*del)(void *));
void			hashmap_free_content(t_hashmap *map, void (*del)(void *));

// hashmap_methods.c
int				hashmap_resize(size_t new_size, t_hashmap *map);
void			*hashmap_search(unsigned long key, t_hashmap *map);
int				hashmap_insert(unsigned long key, void *value, t_hashmap *map);

// hashmap_utils.c
void			hashmap_iterate(t_hashmap *map,
					void (*f) (unsigned long key, void *));
unsigned long	hash(char *str);

#endif