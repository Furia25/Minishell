/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redis_except_here_doc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:49:43 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/22 19:12:25 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_red_input(t_leaf *cmd, t_lexeme_type token_type, char *file)
{
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return ;
	if (cmd->fd_input != STDIN_FILENO)
		close(cmd->fd_input);
	cmd->fd_input = open(file, O_RDONLY);
	if (cmd->fd_input == -1)
		print_basic_error(file);
	if (token_type == HERE_DOC && ft_strncmp(HERE_DOC_FILE, file, 13) == 0)
		unlink(file);
}

void	handle_red_output(t_leaf *cmd, char *file)
{
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return ;
	if (cmd->fd_output != STDOUT_FILENO)
		close(cmd->fd_output);
	cmd->fd_output = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->fd_output == -1)
		print_basic_error(file);
}

void	handle_red_output_append(t_leaf *cmd, char *file)
{
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return ;
	if (cmd->fd_output != STDOUT_FILENO)
		close(cmd->fd_output);
	cmd->fd_output = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->fd_output == -1)
		print_basic_error(file);
}

void	handle_red_input_output(t_leaf *cmd, char *file)
{
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return ;
	if (cmd->fd_input != STDIN_FILENO)
		close(cmd->fd_input);
	cmd->fd_input = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (cmd->fd_input == -1)
		print_basic_error(file);
}
