/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_managing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:05:32 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/21 02:43:17 by val              ###   ########.fr       */
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
	var->exported = true;
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

static t_envvar	*env_instantiate_default(unsigned long hash_key, char *key,
				char *heap_default, t_minishell *data);

t_envvar	*env_instantiate(char *key, char *heap_default, t_minishell *data)
{
	t_hash_entry	*entry;
	t_envvar		*envvar;
	unsigned long	hash_key;

	hash_key = hash(key);
	entry = hashmap_search(hash_key, &data->environment);
	if (entry)
	{
		envvar = (t_envvar *)entry->value;
		free(envvar->value);
		envvar->value = heap_default;
	}
	else
		return (env_instantiate_default(hash_key, key, heap_default, data));
	return (envvar);
}

static t_envvar	*env_instantiate_default(unsigned long hash_key, char *key,
	char *heap_default, t_minishell *data)
{
	char		*dup_key;
	t_envvar	*envvar;

	dup_key = ft_strdup(key);
	if (!dup_key)
		return (NULL);
	envvar = new_envvar(dup_key, heap_default);
	if (!envvar)
	{
		free(dup_key);
		return (NULL);
	}
	if (!hashmap_insert(hash_key, envvar, &data->environment))
	{
		envvar_free(envvar);
		return (NULL);
	}
	return (envvar);
}
