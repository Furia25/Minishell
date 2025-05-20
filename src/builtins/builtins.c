/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:11:13 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/20 15:36:21 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

void	try_builtin(t_builtin_type type, int argc,
	char **argv, t_minishell *data)
{
	if (type == BUILTIN_TYPE_CD)
		data->exit_code = cd_builtin(argc, argv, data);
	else if (type == BUILTIN_TYPE_ECHO)
		data->exit_code = echo_builtin(argc, argv);
	else if (type == BUILTIN_TYPE_ENV)
		data->exit_code = env_builtin(data);
	else if (type == BUILTIN_TYPE_EXPORT)
		data->exit_code = export_builtin(argc, argv, data);
	else if (type == BUILTIN_TYPE_PWD)
		data->exit_code = pwd_builtin();
	else if (type == BUILTIN_TYPE_UNSET)
		data->exit_code = unset_builtin(argc, argv, data);
	else if (type == BUILTIN_TYPE_EXIT)
		data->exit_code = exit_builtin(argc, argv, data);
	if (data->exit_code == BUILTIN_FATAL_ERROR)
	{
		data->exit_code = EXIT_FAILURE;
		exit_minishell(data);
	}
}

t_builtin_type	get_builtin(char *cmd)
{
	t_builtin_type	result;

	if (!cmd)
		return (BUILTIN_TYPE_NOTBUILTIN);
	if (ft_strcmp(cmd, BUILTIN_NAME_CD) == 0)
		result = BUILTIN_TYPE_CD;
	else if (ft_strcmp(cmd, BUILTIN_NAME_ECHO) == 0)
		result = BUILTIN_TYPE_ECHO;
	else if (ft_strcmp(cmd, BUILTIN_NAME_ENV) == 0)
		result = BUILTIN_TYPE_ENV;
	else if (ft_strcmp(cmd, BUILTIN_NAME_EXPORT) == 0)
		result = BUILTIN_TYPE_EXPORT;
	else if (ft_strcmp(cmd, BUILTIN_NAME_PWD) == 0)
		result = BUILTIN_TYPE_PWD;
	else if (ft_strcmp(cmd, BUILTIN_NAME_UNSET) == 0)
		result = BUILTIN_TYPE_UNSET;
	else if (ft_strcmp(cmd, BUILTIN_NAME_EXIT) == 0)
		result = BUILTIN_TYPE_EXIT;
	else
		result = BUILTIN_TYPE_NOTBUILTIN;
	return (result);
}
