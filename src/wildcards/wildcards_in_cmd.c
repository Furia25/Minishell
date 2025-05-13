/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_in_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:54:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/13 22:12:55 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lst *create_and_add_wildcards_nodes(t_lst *prev, t_lst *current, t_leaf *cmd, t_minishell *data)
{
	t_wsearch wildcard_result;
	t_lst *last_wildcards_node;
	
	wildcard_result = wildcard_lst_from_lexeme(current->lexeme);
	if (wildcard_result.code == -1)
		malloc_error(data);
	if (wildcard_result.code == 1)
	{
		if (current == cmd->tokens)
			cmd->tokens = wildcard_result.result;
		else
			prev->next = wildcard_result.result;
		last_wildcards_node = lstlast(wildcard_result.result);
		last_wildcards_node->next = current->next;
		gc_free_node(current, data);
		return (last_wildcards_node);
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
