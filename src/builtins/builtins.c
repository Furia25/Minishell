/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:11:13 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/09 16:16:16 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	try_builtin(t_builtin_type type, int argc, char **v, t_minishell *data)
{
	if (data->in_pipe)
	{
		data->exit_code = EXIT_SUCCESS;
		return (true);
	}
	if (data->in_child)
		return (false);
	if (type == BUILTIN_TYPE_CD)
		data->exit_code = cd_builtin(argc, v, data);
	else if (type == BUILTIN_TYPE_ECHO)
		data->exit_code = echo_builtin(argc, v);
	else if (type == BUILTIN_TYPE_ENV)
		data->exit_code = env_builtin(data);
	else if (type == BUILTIN_TYPE_EXPORT)
		data->exit_code = export_builtin(argc, v, data);
	else if (type == BUILTIN_TYPE_PWD)
		data->exit_code = pwd_builtin();
	else if (type == BUILTIN_TYPE_UNSET)
		data->exit_code = unset_builtin(argc, v, data);
	else if (type == BUILTIN_TYPE_EXIT)
		//result = exit_bultin(argc, v, data);
	if (data->exit_code == BUILTIN_FATAL_ERROR)
		return (false);
	return (true);
}

t_builtin_type	get_builtin(char *cmd)
{
	t_builtin_type	result;

	if (!cmd)
		return (BUILTIN_TYPE_NOTBUILTIN);
	if (ft_strcmp(cmd, "cd") == 0)
		result = BUILTIN_TYPE_CD;
	else if (ft_strcmp(cmd, "echo") == 0)
		result = BUILTIN_TYPE_ECHO;
	else if (ft_strcmp(cmd, "env") == 0)
		result = BUILTIN_TYPE_ENV;
	else if (ft_strcmp(cmd, "export") == 0)
		result = BUILTIN_TYPE_EXPORT;
	else if (ft_strcmp(cmd, "pwd") == 0)
		result = BUILTIN_TYPE_PWD;
	else if (ft_strcmp(cmd, "unset") == 0)
		result = BUILTIN_TYPE_UNSET;
	else if (ft_strcmp(cmd, "exit") == 0)
		result = BUILTIN_TYPE_EXIT;
	else
		result = BUILTIN_TYPE_NOTBUILTIN;
	return (result);
}
