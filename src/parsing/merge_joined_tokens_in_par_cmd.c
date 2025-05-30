/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_joined_tokens_in_par_cmd.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:28:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/30 02:31:44 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_tokens(t_lst *curr_token, t_lst *next_token, t_minishell *data);

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

void	merge_joined_tokens_in_par_cmd(t_lst *tokens, t_minishell *data)
{
	while (tokens)
	{
		if (tokens->next == NULL)
			return ;
		if (is_parenthesized(tokens) == false)
		{
			if ((tokens->type == WORD || tokens->type == DOLLAR
					|| tokens->type == SINGLE_Q || tokens->type == DOUBLE_Q)
				&& tokens->metacharacter_after == false)
			{
				if (tokens->next->type != DOLLAR)
				{
					merge_tokens(tokens, tokens->next, data);
					continue ;
				}
			}
		}
		tokens = tokens->next;
	}
}
