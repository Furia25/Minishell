/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:38:31 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/15 17:01:46 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_command_path(char *cmd, t_hash_entry *path,
	t_minishell *data);
static void	error_free_paths(char **paths, t_minishell *data);

char	*find_command(char *cmd, t_minishell *data)
{
	t_hash_entry	*path;

	if (!cmd)
		return (NULL);
	if (access(cmd, F_OK | X_OK | R_OK) == 0)
		return (cmd);
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

void	command_notfound(char *cmd, t_minishell *data)
{
	if (data->script_mode)
	{
		if (data->script_file)
			ft_putstr_fd(data->script_file, 2);
		else
			ft_putstr_fd("minishell", 2);
		ft_putstr_fd(": line ", 2);
		ft_putnbr_fd(data->line, 2);
		ft_putstr_fd(": ", 2);
	}
	else
		ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(" : command not found\n", 2);
	data->exit_code = 127;
	exit_minishell(data);
}
