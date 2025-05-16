/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:42:39 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/16 17:43:24 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_childs(t_minishell *data)
{
	int	status;

	if (data->last_cmd_pid == -1)
	{
		while (wait(NULL) != -1);
		return ;
	}
	waitpid(data->last_cmd_pid, &status, 0);
	data->exit_code = (status >> 8) & 0xFF;
	while (wait(NULL) != -1);
}

void	close_input_output(t_leaf *cmd)
{
	if (cmd->fd_input != 0 && cmd->fd_input != -1)
		close(cmd->fd_input);
	if (cmd->fd_output != 1 && cmd->fd_output != -1)
		close(cmd->fd_output);
}
