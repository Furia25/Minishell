/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:20:04 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/18 17:32:35 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	env_print_el(unsigned long key, void *entry, bool print_local)
{
	t_envvar	*var;

	(void) key;
	var = (t_envvar *) entry;
	if (!print_local)
	{
		if (var->exported)
			ft_printf("%s=%s\n", var->name, var->value);
	}
	else
	{
		if (var->exported)
			ft_printf("declare -x %s=\"%s\"\n", var->name, var->value);
		else
			ft_printf("declare -x %s\n", var->name);
	}
}

void	env_print(t_hashmap *environment, bool print_local)
{
	size_t			index;
	int				actual_count;
	t_hash_entry	entry;

	index = 0;
	actual_count = 0;
	while (index < environment->size && actual_count < environment->count)
	{
		entry = environment->table[index];
		if (entry.status == OCCUPIED)
		{
			env_print_el(entry.key, entry.value, print_local);
			actual_count++;
		}
		index++;
	}
}
