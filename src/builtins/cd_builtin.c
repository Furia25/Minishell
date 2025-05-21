/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:36:12 by val               #+#    #+#             */
/*   Updated: 2025/05/21 02:50:46 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include <errno.h>

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
	if (argc == 1 || ft_strcmp(argv[1], "~") == 0)
		return (handle_env(ENV_HOME, data));
	if (ft_strcmp(argv[1], "") == 0)
		return (EXIT_SUCCESS);
	if (ft_strcmp(argv[1], "-") == 0)
		return (handle_env(ENV_OLDPWD, data));
	if (chdir(argv[1]) == 0)
	{
		if (!update_pwds(data))
			return (BUILTIN_FATAL_ERROR);
		return (EXIT_SUCCESS);
	}
	ft_printf_fd(2, "%s: %s: %s: %s\n",
		MINISHELL_NAME, BUILTIN_NAME_CD, argv[1], strerror(errno));
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
	if (chdir(((t_envvar *)entry->value)->value) == -1)
	{
		print_basic_error(BUILTIN_NAME_CD);
		return (EXIT_FAILURE);
	}
	if (!update_pwds(data))
	{
		print_basic_error(BUILTIN_NAME_CD);
		return (BUILTIN_FATAL_ERROR);
	}
	return (EXIT_SUCCESS);
}

static bool	update_env_pwds(char *pwd, char *old_pwd, t_minishell *data);

static bool	update_pwds(t_minishell *data)
{
	t_hash_entry	*pwd_entry;
	char			*prev_pwd;
	char			*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (false);
	pwd_entry = hashmap_search(hash(ENV_PWD), &data->environment);
	if (pwd_entry)
		prev_pwd = ((t_envvar *)pwd_entry->value)->value;
	else
		prev_pwd = NULL;
	if (!update_env_pwds(cwd, prev_pwd, data))
	{
		free(cwd);
		return (false);
	}
	return (true);
}

static bool	update_env_pwds(char *pwd, char *old_pwd, t_minishell *data)
{
	t_envvar	*tmp;

	if (old_pwd)
	{
		tmp = env_instantiate(ENV_OLDPWD, ft_strdup(old_pwd), data);
		if (!tmp)
			return (false);
	}
	tmp = env_instantiate(ENV_PWD, pwd, data);
	if (!tmp)
		return (false);
	return (true);
}
