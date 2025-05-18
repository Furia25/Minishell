/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 01:21:44 by val               #+#    #+#             */
/*   Updated: 2025/05/18 23:45:11 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_envvar	*make_pwd(char *key_to_dup, char *value)
{
	t_envvar	*var;
	char		*key;

	if (!value)
		return (NULL);
	key = ft_strdup(key_to_dup);
	if (!key)
		return (NULL);
	var = new_envvar(key, value);
	if (!var)
	{
		free(key);
		return (NULL);
	}
	return (var);
}

t_envvar	*get_pwd(char *pwd_type, char *default_value, t_minishell *data)
{
	t_hash_entry	*entry;
	t_envvar		*pwd;
	unsigned long	hashed;

	hashed = hash(pwd_type);
	entry = hashmap_search(hashed, &data->environment);
	if (!entry)
	{
		pwd = make_pwd(pwd_type, default_value);
		if (!pwd)
			return (NULL);
		if (!hashmap_insert(hashed, pwd, &data->environment))
		{
			free(pwd->name);
			free(pwd);
			return (NULL);
		}
		return (pwd);
	}
	return ((t_envvar *) entry->value);
}
