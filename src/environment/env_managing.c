/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_managing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:05:32 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/11 20:00:50 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	envvar_free(void *el)
{
	t_envvar	*var;

	var = (t_envvar *) el;
	if (var->name)
		free(var->name);
	if (var->value)
		free(var->value);
	free(var);
}

bool	env_populate(char **envp, t_hashmap *environment)
{
	size_t			i;
	size_t			j;
	t_envvar		*var;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j++])
		{
			if (envp[i][j] != '=')
				continue ;
			var = ft_calloc(1, sizeof(t_envvar));
			if (!var)
				return (false);
			var->name = ft_substr(envp[i], 0, j);
			if (!var->name)
				return (envvar_free(var), false);
			var->value = ft_substr(envp[i], j + 1, ft_strlen(envp[i]));
			if (!hashmap_insert(hash(var->name), var, environment))
				return (envvar_free(var), false);
			break;
		}
		i++;
	}
	return (true);
}

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
