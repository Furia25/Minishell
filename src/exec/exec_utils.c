/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/22 02:54:24 by alpayet          ###   ########.fr       */
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

void	secure_close_input_output(t_leaf *cmd)
{
	if (cmd->fd_input > STDIN_FILENO)
	{
		secure_close(cmd->fd_input);
		cmd->fd_input = STDIN_FILENO;
	}
	if (cmd->fd_output > STDOUT_FILENO)
	{
		secure_close(cmd->fd_output);
		cmd->fd_output = STDOUT_FILENO;
	}
}

void 	secure_close(int fd)
{
	if (close(fd) == -1)
		perror("minishell : close");
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
