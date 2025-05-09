/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:11:13 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/09 15:40:41 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	try_builtin(t_builtin_type type, int argc, char **argv, t_minishell *data)
{
	int	result;

	if (data->in_child)
		return (false);
	if (type == BUILTIN_TYPE_CD)
		result = cd_builtin(argc, argv, data);
	else if (type == BUILTIN_TYPE_ECHO)
		result = echo_builtin(argc, argv);
	else if (type == BUILTIN_TYPE_ENV)
		result = env_builtin(data);
	else if (type == BUILTIN_TYPE_EXPORT)
		result = export_builtin(argc, argv, data);
	else if (type == BUILTIN_TYPE_PWD)
		result = pwd_builtin();
	else if (type == BUILTIN_TYPE_UNSET)
		result = unset_builtin(argc, argv, data);
	else if (type == BUILTIN_TYPE_EXIT)
		//result = exit_bultin(argc, argv, data);
	if (result == BUILTIN_FATAL_ERROR)
		return (false);
	data->exit_code = result;
	return (true);
}

t_builtin_type	get_builtin(char *cmd)
{
	t_builtin_type	result;

	if (ft_strcmp(cmd, "cd"))
		result = BUILTIN_TYPE_CD;
	else if (ft_strcmp(cmd, "echo"))
		result = BUILTIN_TYPE_ECHO;
	else if (ft_strcmp(cmd, "env"))
		result = BUILTIN_TYPE_ENV;
	else if (ft_strcmp(cmd, "export"))
		result = BUILTIN_TYPE_EXPORT;
	else if (ft_strcmp(cmd, "pwd"))
		result = BUILTIN_TYPE_PWD;
	else if (ft_strcmp(cmd, "unset"))
		result = BUILTIN_TYPE_UNSET;
	else if (ft_strcmp(cmd, "exit"))
		result = BUILTIN_TYPE_EXIT;
	else
		result = BUILTIN_TYPE_NOTBUILTIN;
	return (result);
}
