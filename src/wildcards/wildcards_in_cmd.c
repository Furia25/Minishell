/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_in_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:54:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/17 19:38:41 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_wildcard_to_gc(t_wsearch wildcard_result, t_minishell *data);

static	bool have_wildcard(char	*str)
{
	while (ft_strchr("*?", *str) == NULL)
		str++;
	if (*str == '\0')
		return (false);
	else
		return (true);
}
static t_lst *create_and_add_wildcards_nodes(t_lst *prev, t_lst *current,
	t_leaf *cmd, t_minishell *data)
{
	t_wsearch	wildcard_result;
	t_lst		*last_wildcards_node;

	if (current == NULL)
		return (NULL);
	if (have_wildcard(current->lexeme) == false)
		return (current);
	wildcard_result = wildcard_lst_from_lexeme(current->lexeme);
	if (wildcard_result.code == -1)
		malloc_error(data);
	add_wildcard_to_gc(wildcard_result, data);
	if (wildcard_result.code == 1)
	{
		if (prev == NULL)
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
	if (tokens == NULL)
		return ;
	while (tokens != NULL && tokens->next != NULL)
		tokens = create_and_add_wildcards_nodes(tokens, tokens->next, cmd, data);
}

static void	add_wildcard_to_gc(t_wsearch wildcard_result, t_minishell *data)
{
	while(wildcard_result.result)
	{
		gc_add(wildcard_result.result->lexeme, data);
		gc_add(wildcard_result.result, data);
		wildcard_result.result = wildcard_result.result->next;
	}
}
