/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:38:31 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/29 17:35:25 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_command_path(char *command, t_hash_entry *path);

char	*find_command(char *cmd, t_hashmap *env)
{
	t_hash_entry	*path;

	if (!cmd)
		return (NULL);
	if (access(cmd, F_OK | X_OK | R_OK) == 0)
		return (cmd);
	path = hashmap_search(hash(ENV_PATH), env);
	if (path != NULL)
		return (find_command_path(cmd, path));
	return (NULL);
}

static char	*find_command_path(char *command, t_hash_entry *path)
{
	size_t			index;
	char			**paths;
	char			*cmd_path;

	paths = ft_split(((t_envvar *) path->value)->value, ':');
	if (!paths)
		return (NULL);
	index = 0;
	while (paths[index])
	{
		cmd_path = ft_strjoin(paths[index], "/");
		cmd_path = ft_strjoin_alt(cmd_path, command, FREE_PARAM1);
		if (!cmd_path)
			break ;
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_chartab(paths);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free_chartab(paths);
	return (NULL);
}
