/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_in_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:54:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/13 15:49:10 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lst *create_and_add_wildcards_nodes(t_lst *prev, t_lst *current, t_leaf *cmd, t_minishell *data)
{
	t_wsearch wildcard_result;
	
	wildcard_result = wildcard_lst_from_lexeme(current->lexeme);
	if (wildcard_result.code == -1)
		malloc_error(data);
	if (wildcard_result.code == 1)
	{
		if (current == cmd->tokens)
			cmd->tokens = wildcard_result.result;
		else
			prev->next = wildcard_result.result;
		lstlast(wildcard_result.result)->next = current->next;
		gc_free_node(current, data);
		return (lstlast(wildcard_result.result));
	}
	return (current);
}

void	wildcards_in_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst	*tokens;
	
	tokens = create_and_add_wildcards_nodes(NULL, cmd->tokens, cmd, data);
	while (tokens->next != NULL)
		tokens = create_and_add_wildcards_nodes(tokens, tokens->next, cmd, data);
}
