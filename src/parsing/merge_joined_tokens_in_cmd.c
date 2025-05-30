/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_joined_tokens_in_cmd.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:28:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/30 02:00:08 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_tokens(t_lst *curr_token, t_lst *next_token, t_minishell *data)
{
	curr_token->lexeme = ft_strjoin_alt_gc(curr_token->lexeme,
			next_token->lexeme, FREE_PARAM1, data);
	check_malloc(curr_token->lexeme, data);
	curr_token->type = next_token->type;
	curr_token->metacharacter_after = next_token->metacharacter_after;
	curr_token->next = next_token->next;
	gc_free_node(next_token, data);
}

void	merge_joined_tokens_eof(t_lst *tokens, t_minishell *data)
{
	while (tokens && tokens->metacharacter_after == false)
	{
		if (tokens->next == NULL)
			return ;
		if ((tokens->type == WORD
				|| tokens->type == SINGLE_Q || tokens->type == DOUBLE_Q)
			&& tokens->metacharacter_after == false)
		{
			merge_tokens(tokens, tokens->next, data);
			continue ;
		}
		tokens = tokens->next;
	}
}

void	merge_joined_tokens_in_cmd(t_lst *tokens, t_minishell *data)
{
	while (tokens)
	{
		if (tokens->next == NULL)
			return ;
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
		tokens = tokens->next;
	}
}
