/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_in_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:54:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/13 10:49:31 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wildcards_in_first_tokens_node(t_leaf *cmd, t_minishell *data)
{
	t_wsearch wildcard_result;
	t_lst	*buff_next;
	
	wildcard_result = wildcard_lst_from_lexeme(cmd->tokens->lexeme);
	if (wildcard_result.code == -1)
		malloc_error(data);
	if (wildcard_result.code == 1)
	{
		buff_next = cmd->tokens->next;
		cmd->tokens = wildcard_result.result;
		lstlast(wildcard_result.result)->next = buff_next;
	
	}
}

void	wildcards_in_cmd(t_leaf *cmd, t_minishell *data)
{
	t_wsearch wildcard_result;
	t_lst	*buff_next;
	t_lst	*tokens;

	wildcards_in_first_tokens_node(cmd, data);
	tokens = cmd->tokens;
	while (tokens->next != NULL)
	{
		wildcard_result = wildcard_lst_from_lexeme(tokens->next->lexeme);
		if (wildcard_result.code == -1)
			malloc_error(data);
		if (wildcard_result.code == 1)
		{
			buff_next = tokens->next->next;
			tokens->next = wildcard_result.result;
			lstlast(wildcard_result.result)->next = buff_next;
		
		}
		tokens = tokens->next;
	}
}
