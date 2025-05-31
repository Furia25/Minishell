/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:11:09 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/31 19:41:21 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "environment.h"

static char	*search_command(char *cmd, t_minishell *data);
static char	*find_command_path(char *cmd, char *path, t_minishell *data);
static void	error_free_paths(char **paths, t_minishell *data);
static char	*find_absolute(char *cmd, t_minishell *data);

void	exec_command(t_leaf *cmd, char **argv, t_minishell *data)
{
	char	*command_path;

	if (data->in_pipe)
		exec_builtins(cmd, argv, true, data);
	data->exit_code = 127;
	command_path = search_command(argv[0], data);
	if (!command_path)
		command_notfound(argv[0], data);
	gc_add(command_path, data);
	setup_signals(SIGCONTEXT_FORK);
	execve(command_path, argv, data->environment_tab);
	data->exit_code = 126;
	print_extended_error(NULL, command_path, strerror(errno));
	exit_minishell(data);
}

static char	*search_command(char *cmd, t_minishell *data)
{
	t_hash_entry	*path;
	char			*temp_cmd;

	if (!cmd || !(*cmd))
		return (NULL);
	if (ft_strchr(cmd, '/') != NULL)
		return (find_absolute(cmd, data));
	path = hashmap_search(hash(ENV_PATH), &data->environment);
	if (path != NULL)
		return (find_command_path(cmd,
				((t_envvar *) path->value)->value, data));
	if (!data->use_hard_path)
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
	}
	else
	{
		temp_cmd = find_command_path(cmd, MINISHELL_HARD_PATH, data);
		if (temp_cmd != NULL)
			return (temp_cmd);
	}
	print_extended_error(NULL, cmd, "No such file or directory");
	exit_minishell(data);
	return (NULL);
}

static char	*find_command_path(char *cmd, char *path, t_minishell *data)
{
	size_t			index;
	char			**paths;
	char			*cmd_path;

	paths = ft_split(path, ':');
	if (!paths)
		raise_error(data);
	index = 0;
	while (paths[index])
	{
		cmd_path = ft_strjoin(paths[index], "/");
		cmd_path = ft_strjoin_alt(cmd_path, cmd, FREE_PARAM1);
		if (!cmd_path)
			error_free_paths(paths, data);
		if (access(cmd_path, F_OK) == 0)
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

static char	*find_absolute(char *cmd, t_minishell *data)
{
	if (!is_directory(cmd))
	{
		if (access(cmd, F_OK) != 0)
			return (NULL);
		return (cmd);
	}
	else
	{
		data->exit_code = 126;
		print_extended_error(NULL, cmd, "is a directory");
		return (NULL);
	}
}
