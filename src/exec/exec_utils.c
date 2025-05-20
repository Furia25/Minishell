/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:42:39 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/21 00:30:43 by alpayet          ###   ########.fr       */
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

bool	exec_builtins(char **argv, bool exit, t_minishell *data)
{
	t_builtin_type	type;
	size_t			argc;

	argc = tab_size(argv);
	type = get_builtin(argv[0]);
	if (type != BUILTIN_TYPE_NOTBUILTIN)
	{
		try_builtin(type, argc, argv, data);
		if (exit)
			exit_minishell(data);
		return (true);
	}
	return (false);
}
