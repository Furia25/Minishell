/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_managing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:05:32 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/14 15:45:56 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envvar	*new_envvar(char *key, char *value)
{
	t_envvar		*var;

	if (!key)
		return (NULL);
	if (!value)
		return (NULL);
	var = ft_calloc(1, sizeof(t_envvar));
	if (!var)
		return (NULL);
	var->name = key;
	var->value = value;
	return (var);
}

void	envvar_free(void *el)
{
	t_envvar	*var;

	if (!el)
		return ;
	var = (t_envvar *) el;
	if (var->name)
		free(var->name);
	if (var->value)
		free(var->value);
	free(var);
}

static bool	add_envvar(size_t j, char *str, t_hashmap *env);

bool	env_populate(char **envp, t_hashmap *environment)
{
	size_t			i;
	size_t			j;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j++])
		{
			if (envp[i][j] != '=')
				continue ;
			if (!add_envvar(j, envp[i], environment))
				return (false);
			break;
		}
		i++;
	}
	return (true);
}

static bool	add_envvar(size_t j, char *str, t_hashmap *env)
{
	t_envvar	*var;
	char		*key;
	char		*value;

	key = ft_substr(str, 0, j);
	if (!key)
		return (false);
	value = ft_substr(str, j + 1, ft_strlen(str));
	if (!value)
	{
		free(key);
		return (false);
	}
	var = new_envvar(key, value);
	if (!hashmap_insert(hash(var->name), var, env))
	{
		envvar_free(var);
		return (false);
	}
	return (true);
}
