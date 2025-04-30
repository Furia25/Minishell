/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:56:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/30 15:20:04 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	handle_red_input(t_leaf *command_tab, char *file, t_minishell *data);
void	handle_red_output(t_leaf *command_tab, char *file, t_minishell *data);
void	handle_red_output_append(t_leaf *command_tab, char *file, t_minishell *data);
void	handle_red_input_output(t_leaf *command_tab, char *file, t_minishell *data);
void	rm_here_doc_files(t_lst	*tokens);

static bool	check_redi(t_leaf *command_tab, t_lst *token, t_minishell *data)
{
	if (token->type == RED_IN || token->type == RED_OUT
		|| token->type == RED_OUT_A || token->type == RED_IN_OUT)
	{
		if (token->type == RED_IN)
			handle_red_input(command_tab, token->next->lexeme, data);
		if (token->type == RED_OUT)
			handle_red_output(command_tab, token->next->lexeme, data);
		if (token->type == RED_OUT_A)
			handle_red_output_append(command_tab, token->next->lexeme, data);
		if (token->type == RED_IN_OUT)
			handle_red_input_output(command_tab, token->next->lexeme, data);
		return (true);
	}
	return (false);
}

static void	del_reds_tokens(t_lst *token, t_minishell *data)
{
	gc_free_node(token->next, data);
	gc_free_node(token, data);
}

void	handle_reds_and_del(t_leaf *command_tab, t_minishell *data)
{
	t_lst	*temp;
	t_lst	*prev;

	while (command_tab->tokens != NULL
		&& check_redi(command_tab, command_tab->tokens, data) == true)
	{
		temp = command_tab->tokens;
		command_tab->tokens = command_tab->tokens->next->next;
		del_reds_tokens(temp, data);
	}
	temp = command_tab->tokens;
	while (temp)
	{
		if (check_redi(command_tab, temp, data) == true)
		{
			prev->next = temp->next->next;
			del_reds_tokens(temp, data);
			temp = prev->next;
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}
}

void	handle_reds_cmds(t_leaf *command_tab, t_minishell *data)
{
	while (command_tab->ope_after != LINE_CHANGE)
		handle_reds_and_del(command_tab, data);
	handle_reds_and_del(command_tab, data);
	while (command_tab->ope_after != LINE_CHANGE)
	{
		if (command_tab->fd_input == -1 || command_tab->fd_output == -1)
			exit(1);
	}
	if (command_tab->fd_input == -1 || command_tab->fd_output == -1)
		exit(1);
}

void	close_all_fds(t_leaf *command_tab)
{
	while (command_tab->ope_after != LINE_CHANGE)
	{
		if (command_tab->fd_input != 0)
			close(command_tab->fd_input);
		if (command_tab->fd_output != 1)
			close(command_tab->fd_output);
		rm_here_doc_files(command_tab->tokens);
		command_tab++;
	}
	if (command_tab->fd_input != 0)
		close(command_tab->fd_input);
	if (command_tab->fd_output != 1)
		close(command_tab->fd_output);
	rm_here_doc_files(command_tab->tokens);
}

