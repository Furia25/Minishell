/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:11:13 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/09 15:33:20 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	try_builtin(int argc, char **argv, t_minishell *data)
{
	int	result;

	if (data->in_child)
		return (false);
	if (ft_strcmp(argv[0], "cd"))
		result = cd_builtin(argc, argv, data);
	else if (ft_strcmp(argv[0], "echo"))
		result = echo_builtin(argc, argv);
	else if (ft_strcmp(argv[0], "env"))
		result = env_builtin(data);
	else if (ft_strcmp(argv[0], "export"))
		result = export_builtin(argc, argv, data);
	else if (ft_strcmp(argv[0], "pwd"))
		result = pwd_builtin();
	else if (ft_strcmp(argv[0], "unset"))
		result = unset_builtin(argc, argv, data);
	else if (ft_strcmp(argv[0], "exit"))
		//result = exit_bultin(argc, argv, data);
	if (result == BUILTIN_FATAL_ERROR)
		return (false);
	data->exit_code = result;
	return (true);
}
