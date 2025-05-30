/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_in_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:56:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/30 01:40:52 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"

void	rm_here_doc_files_in_cmd(t_lst *tokens);

static bool	is_redi(t_leaf *cmd, t_lst *token)
{
	if (token->type == RED_IN || token->type == HERE_DOC
		|| token->type == RED_OUT || token->type == RED_OUT_A
		|| token->type == RED_IN_OUT)
	{
		if (token->next == NULL)
		{
			cmd->fd_input = -1;
			cmd->fd_output = -1;
			ft_printf_fd(STDERR_FILENO,"%s: ambiguous redirect\n", MINISHELL_NAME);
			return (false);
		}
		if (token->type == RED_IN || token->type == HERE_DOC)
			handle_red_input(cmd, token->type, token->next->lexeme);
		if (token->type == RED_OUT)
			handle_red_output(cmd, token->next->lexeme);
		if (token->type == RED_OUT_A)
			handle_red_output_append(cmd, token->next->lexeme);
		if (token->type == RED_IN_OUT)
			handle_red_input_output(cmd, token->next->lexeme);
		return (true);
	}
	return (false);
}

void	del_reds_tokens(t_lst *token, t_minishell *data)
{
	gc_free_node(token->next, data);
	gc_free_node(token, data);
}

void	redirections_in_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst	*curr;
	t_lst	*prev;

	while (cmd->tokens != NULL
		&& is_redi(cmd, cmd->tokens) == true)
	{
		curr = cmd->tokens;
		cmd->tokens = cmd->tokens->next->next;
		del_reds_tokens(curr, data);
	}
	curr = cmd->tokens;
	while (curr)
	{
		if (is_redi(cmd, curr) == true)
		{
			prev->next = curr->next->next;
			del_reds_tokens(curr, data);
			curr = prev->next;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

void	close_all_fds(t_leaf *command_tab)
{
	while (command_tab->ope_after != LINE_CHANGE)
	{
		if (command_tab->fd_input > STDIN_FILENO)
			close(command_tab->fd_input);
		if (command_tab->fd_output > STDOUT_FILENO)
			close(command_tab->fd_output);
		rm_here_doc_files_in_cmd(command_tab->tokens);
		command_tab++;
	}
	if (command_tab->fd_input > STDIN_FILENO)
		close(command_tab->fd_input);
	if (command_tab->fd_output > STDOUT_FILENO)
		close(command_tab->fd_output);
	rm_here_doc_files_in_cmd(command_tab->tokens);
}
