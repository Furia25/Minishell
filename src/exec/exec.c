/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/21 20:24:51 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell_signal.h"
#include "minishell.h"
#include <sys/stat.h>
#include <unistd.h>

static char	*search_command(char *cmd, t_minishell *data);
static char	*find_command_path(char *cmd, t_hash_entry *path,
				t_minishell *data);
static void	error_free_paths(char **paths, t_minishell *data);

void	exec_command(t_leaf *cmd, char **argv, t_minishell *data)
{
	char	*command_path;

	if (data->in_pipe)
		exec_builtins(cmd, argv, true, data);
	data->exit_code = 127;
	command_path = search_command(argv[0], data);
	if (!command_path)
		command_notfound(argv[0], data);
	setup_signals(SIGCONTEXT_FORK);
	execve(command_path, argv, data->environment_tab);
	free(command_path);
	exit_minishell(data);
}

static char	*search_command(char *cmd, t_minishell *data)
{
	t_hash_entry	*path;

	if (!cmd || !(*cmd))
		return (NULL);
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (!is_directory(cmd))
		{
			if (access(cmd, F_OK) != 0)
				return (NULL);
			if (access(cmd, X_OK) == 0)
				return (cmd);
			data->exit_code = 126;
			return (NULL);
		}
		else
		{
			ft_printf_fd(STDERR_FILENO, "%s: %s is a directory\n", MINISHELL_NAME, cmd);
			return (NULL);
		}
	}
	path = hashmap_search(hash(ENV_PATH), &data->environment);
	if (path != NULL)
		return (find_command_path(cmd, path, data));
	return (NULL);
}

static char	*find_command_path(char *cmd, t_hash_entry *path, t_minishell *data)
{
	size_t			index;
	char			**paths;
	char			*cmd_path;

	paths = ft_split(((t_envvar *) path->value)->value, ':');
	if (!paths)
		raise_error(data);
	index = 0;
	while (paths[index])
	{
		cmd_path = ft_strjoin(paths[index], "/");
		cmd_path = ft_strjoin_alt(cmd_path, cmd, FREE_PARAM1);
		if (!cmd_path)
			error_free_paths(paths, data);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_chartab(paths);
			return (cmd_path);
		}
		free(cmd_path);
		index++;
	}
	free_chartab(paths);
	return (NULL);
}

static void	error_free_paths(char **paths, t_minishell *data)
{
	free_chartab(paths);
	raise_error(data);
}
