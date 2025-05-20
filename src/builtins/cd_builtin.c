/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:42:00 by val               #+#    #+#             */
/*   Updated: 2025/05/20 15:09:31 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

static int	handle_env(char *env, t_minishell *data);
static bool	update_pwds(t_minishell *data);

int	cd_builtin(int argc, char **argv, t_minishell *data)
{
	if (argc > 2)
	{
		ft_printf_fd(2, "%s: %s: %s\n",
			MINISHELL_NAME, BUILTIN_NAME_CD, BUILTIN_ERROR_TOOMANY);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
		return (handle_env(ENV_HOME, data));
	if (ft_strlen(argv[1]) == 0)
		return (EXIT_SUCCESS);
	if (ft_strcmp(argv[1], "~") == 0)
		return (handle_env(ENV_HOME, data));
	if (ft_strcmp(argv[1], "-") == 0)
		return (handle_env(ENV_OLDPWD, data));
	if (chdir(argv[1]) == 0)
	{
		if (!update_pwds(data))
			return (BUILTIN_FATAL_ERROR);
		return (EXIT_SUCCESS);
	}
	print_basic_error(BUILTIN_NAME_CD);
	return (EXIT_FAILURE);
}

static int	handle_env(char *env, t_minishell *data)
{
	t_hash_entry	*entry;

	entry = hashmap_search(hash(env), &data->environment);
	if (!entry)
	{
		ft_printf_fd(2, "%s: %s: %s not set\n",
			MINISHELL_NAME, BUILTIN_NAME_CD, env);
		return (EXIT_FAILURE);
	}
	else
	{
		if (chdir(((t_envvar *) entry->value)->value) == -1)
		{
			ft_putstr_fd("ZIZI", 2);
			print_basic_error(BUILTIN_NAME_CD);
			return (EXIT_FAILURE);
		}
		if (!update_pwds(data))
		{
			print_basic_error(BUILTIN_NAME_CD);
			return (BUILTIN_FATAL_ERROR);
		}
	}
	return (EXIT_SUCCESS);
}

static bool	update_pwds(t_minishell *data)
{
	t_envvar		*old_pwd;
	t_envvar		*pwd;
	char			*new_path;
	char			*last;

	new_path = getcwd(NULL, 0);
	if (!new_path)
		return (false);
	pwd = get_pwd(ENV_PWD, new_path, data);
	if (!pwd)
	{
		free(new_path);
		return (false);
	}
	if (pwd->value == new_path)
		return (true);
	last = pwd->value;
	pwd->value = new_path;
	old_pwd = get_pwd(ENV_OLDPWD, last, data);
	if (!old_pwd)
		return (false);
	if (old_pwd->value != last)
		old_pwd->value = last;
	return (true);
}
