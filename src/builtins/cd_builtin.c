/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:42:00 by val               #+#    #+#             */
/*   Updated: 2025/04/18 19:45:20 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_home(t_minishell *data);

int	cd_builtin(int argc, char **argv, t_minishell *data)
{
	(void) argv;
	if (argc == 1)
		return (handle_home(data));
	if (ft_strcmp(argv[1], "~"))
		return (handle_home(data));
	if (ft_strcmp(argv[1], "-"))
	{
		
	}
	return (EXIT_FAILURE);
}

static bool	handle_home(t_minishell *data)
{
	t_hash_entry	*entry;
	char			*new_path;
	t_envvar		*var;

	entry = hashmap_search(hash(ENV_HOME), &data->environment);
	if (!entry)
	{
		write_str_secure(BUILTIN_ERROR_CD_HOME, 2);
		return (EXIT_FAILURE);
	}
	else
	{
		var = (t_envvar *) entry->value;
		if (chdir(var->value) == -1)
		{
			perror(BUILTIN_ERROR_CD);
			return (EXIT_FAILURE);
		}
		update_pwds()
	}
	return (EXIT_SUCCESS);
}

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

static t_envvar	*set_pwd(char *pwd_type, char *new, t_minishell *data)
{
	t_envvar		*pwd;
	unsigned long	hashed;

	hashed = hash(pwd_type);
	pwd = hashmap_search(hashed, &data->environment);
	if (!pwd)
	{
		pwd = make_pwd(pwd_type, new);
		if (!pwd)
		{
			free(new);
			return (NULL);
		}
		if (!hashmap_insert(hashed, pwd, &data->environment))
		{
			envvar_free(pwd);
			return (NULL);
		}
		return (pwd);
	}
	return (pwd);
}

static bool	update_pwds(t_minishell *data)
{
	t_hash_entry	*temp;
	t_envvar		*old_pwd;
	char			*new_path;

	new_path = getcwd(NULL, 0);
	if (!new_path)
		return (false);
	set_pwd(ENV_PWD, new_path, data);
	
	return (true);
}
