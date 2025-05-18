/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_populate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:02:46 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/18 23:50:12 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	add_envvar(char *key, char *value, t_hashmap *env);
static bool	add_envvar_stack(char *key, char *value, t_hashmap *env);
static bool	env_add_mandatories(t_hashmap *environment);

bool	env_populate(char **envp, t_hashmap *environment)
{
	size_t			i;
	size_t			j;
	char			*key;
	char			*value;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j++])
		{
			if (envp[i][j] != '=')
				continue ;
			key = ft_substr(envp[i], 0, j);
			value = ft_substr(envp[i], j + 1, ft_strlen(envp[i]));
			if (!add_envvar(key, value, environment))
				return (false);
			break ;
		}
		i++;
	}
	if (!env_add_mandatories(environment))
		return (false);
	return (true);
}

static bool	env_add_mandatories(t_hashmap *environment)
{
	t_hash_entry	*entry;
	t_envvar		*value;
	int				int_temp;

	entry = hashmap_search(hash(ENV_SHLVL), environment);
	if (entry != NULL)
	{
		value = (t_envvar *) entry->value;
		int_temp = ft_atoi(value->value);
		free(value->value);
		value->value = ft_itoa(int_temp + 1);
		if (!value->value)
			return (false);
	}
	else
		if (!add_envvar_stack(ENV_SHLVL, "1", environment))
			return (false);
	if (!hashmap_search(hash(ENV_PWD), environment))
	{
		if (!add_envvar(ft_strdup(ENV_PWD), getcwd(NULL, 0), environment))
			return (false);
	}
	return (true);
}

static bool	add_envvar_stack(char *key, char *value, t_hashmap *env)
{
	key = ft_strdup(key);
	value = ft_strdup(value);
	if (!add_envvar(key, value, env))
		return (false);
	return (true);
}

static bool	add_envvar(char *key, char *value, t_hashmap *env)
{
	t_envvar	*temp;

	if (!value || !key)
	{
		if (key)
			free(key);
		if (value)
			free(value);
		return (false);
	}
	temp = new_envvar(key, value);
	if (!temp)
	{
		free(key);
		free(value);
		return (false);
	}
	if (!hashmap_insert(hash(key), temp, env))
	{
		envvar_free(temp);
		return (false);
	}
	return (true);
}
