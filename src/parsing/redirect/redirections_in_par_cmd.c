/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_in_par_cmd.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 03:56:04 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/31 15:28:06 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"

void	rm_here_doc_files_in_cmd(t_lst *tokens);
void	del_reds_tokens(t_lst *token, t_minishell *data);

static bool	is_parenthesized(t_lst *token)
{
	static bool	in_parenthesis = false;

	if (token == NULL)
		return (false);
	if (token->type == PAR_OPEN)
		in_parenthesis = true;
	if (token->type == LAST_PAR_CLOSE)
		in_parenthesis = false;
	if (in_parenthesis == true || token->type == LAST_PAR_CLOSE)
		return (true);
	return (in_parenthesis);
}

static bool	is_redi(t_leaf *cmd, t_lst *token)
{
	if (is_parenthesized(token) == true)
		return (false);
	if (token->type == RED_IN || token->type == HERE_DOC
		|| token->type == RED_OUT || token->type == RED_OUT_A
		|| token->type == RED_IN_OUT)
	{
		if (token->next == NULL)
		{
			cmd->fd_input = -1;
			cmd->fd_output = -1;
			ft_printf_fd(STDERR_FILENO, "%s: ambiguous redirect\n",
				MINISHELL_NAME);
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

void	redirections_in_par_cmd(t_leaf *cmd, t_minishell *data)
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
