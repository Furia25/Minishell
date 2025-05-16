/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:38:31 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/16 16:53:45 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <unistd.h>

static char *search_command(char *cmd, t_minishell *data);
static char	*find_command_path(char *cmd, t_hash_entry *path,
	t_minishell *data);
static void	error_free_paths(char **paths, t_minishell *data);

void	exec_command(char **argv, t_minishell *data)
{
	char	*command_path;

	command_path = find_command(argv[0], data);
	if (!command_path)
		command_notfound(argv[0], data);
	execve(command_path, argv, data->environment_tab);
	free(command_path);
	exit_minishell(data);
}

char	*find_command(char *cmd, t_minishell *data)
{
	char			*command_finded;
	struct stat		file_data;

	ft_memset(&file_data, 0, sizeof(struct stat));
	command_finded = search_command(cmd, data);
	if (!command_finded)
		return (NULL);
	lstat(command_finded, &file_data);
	if (!S_ISDIR(file_data.st_mode))
		return (command_finded);
	else
		return (NULL);
}

static char *search_command(char *cmd, t_minishell *data)
{
	t_hash_entry	*path;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (cmd);
		return (NULL);
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
		malloc_error(data);
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
	malloc_error(data);
}
