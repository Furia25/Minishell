/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_cmd_substitution_in_par_cmd.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:11:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/06/02 10:39:58 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_dollars_changes_in_lexeme(t_lst *token, t_minishell *data);
t_lst	*create_and_add_dollars_nodes(t_lst *prev, t_lst *current,
			t_leaf *cmd, t_minishell *data);

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

void	ev_expand_cmd_substitution_in_par_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst	*temp;

	temp = cmd->tokens;
	if (is_parenthesized(temp) == false)
	{
		if (temp->type == WORD || temp->type == DOUBLE_Q)
			add_dollars_changes_in_lexeme(temp, data);
		if (temp->type == WORD)
			temp = create_and_add_dollars_nodes(NULL, temp, cmd, data);
	}
	while (temp != NULL && temp->next)
	{
		if (is_parenthesized(temp->next) == false)
		{
			if (temp->next->type == WORD || temp->next->type == DOUBLE_Q)
				add_dollars_changes_in_lexeme(temp->next, data);
			if (temp->next->type == WORD)
				temp = create_and_add_dollars_nodes(temp,
						temp->next, cmd, data);
			else
				temp = temp->next;
		}
		else
			temp = temp->next;
	}
}
