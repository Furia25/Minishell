/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/21 03:14:47 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builtin.h"
#include "minishell.h"

void	wait_childs(t_minishell *data)
{
	pid_t	temp_wait;
	int		status;

	if (data->last_cmd_pid == -1)
	{
		temp_wait = wait(NULL);
		while (temp_wait != -1)
			temp_wait = wait(NULL);
		return ;
	}
	waitpid(data->last_cmd_pid, &status, 0);
	data->exit_code = (status >> 8) & 0xFF;
	temp_wait = wait(NULL);
	while (temp_wait != -1)
		temp_wait = wait(NULL);
}

void	close_input_output(t_leaf *cmd)
{
	if (cmd->fd_input > STDIN_FILENO)
		close(cmd->fd_input);
	if (cmd->fd_output > STDOUT_FILENO)
		close(cmd->fd_output);
}

bool	exec_builtins(t_leaf *cmd, char **argv, bool exit, t_minishell *data)
{
	bool			result;
	size_t			argc;

	argc = tab_size(argv);
	result = try_builtin(cmd, argc, argv, data);
	if (data->exit_code == BUILTIN_FATAL_ERROR)
	{
		data->exit_code = EXIT_FAILURE;
		exit_minishell(data);
	}
	if (exit && result)
		exit_minishell(data);
	return (result);
}
