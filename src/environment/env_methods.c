/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:20:04 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/17 16:36:09 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	env_print_el(unsigned long key, void *entry)
{
	t_envvar	*var;

	(void) key;
	var = (t_envvar *) entry;
	ft_printf("%s=%s\n", var->name, var->value);
}

void	env_print(t_hashmap *environment)
{
	hashmap_iterate(environment, env_print_el);
}
