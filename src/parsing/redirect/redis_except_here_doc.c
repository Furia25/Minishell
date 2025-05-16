/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redis_except_here_doc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:49:43 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/16 16:36:06 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_red_input(t_leaf *cmd, char *file, t_minishell *data)
{
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return ;
	if (cmd->fd_input != 0)
		close(cmd->fd_input);
	cmd->fd_input = open(file, O_RDONLY);
	if (cmd->fd_input == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
	}
	if (ft_strncmp("/tmp/here_doc", file, 13) == 0)
		unlink(file);
}

void	handle_red_output(t_leaf *cmd, char *file, t_minishell *data)
{
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return ;
	if (cmd->fd_output != 1)
		close(cmd->fd_output);
	cmd->fd_output = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->fd_output == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
	}
}

void	handle_red_output_append(t_leaf *cmd, char *file, t_minishell *data)
{
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return ;
	if (cmd->fd_output != 1)
		close(cmd->fd_output);
	cmd->fd_output = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->fd_output == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
	}
}

void	handle_red_input_output(t_leaf *cmd, char *file, t_minishell *data)
{
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return ;
	if (cmd->fd_input != 1)
		close(cmd->fd_input);
	cmd->fd_input = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (cmd->fd_input == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
	}
}

