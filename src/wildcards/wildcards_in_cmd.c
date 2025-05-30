/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_in_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:54:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/30 02:02:11 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcards.h"

static void	add_wildcard_to_gc(t_wsearch wildcard_result, t_minishell *data);
static void	clean_current_token(char *token);

static bool	have_wildcard(char *str)
{
	while (str && *str)
	{
		if (*str == '?' || *str == '*' || *str == - '*' || *str == - '?')
			return (true);
		str++;
	}
	return (false);
}

t_lst	*create_add_wildcards_nodes(t_lst *prev, t_lst *current,
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
		raise_error(data);
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
	clean_current_token(current->lexeme);
	return (current);
}

void	wildcards_in_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst	*tokens;

	tokens = create_add_wildcards_nodes(NULL, cmd->tokens, cmd, data);
	while (tokens != NULL && tokens->next != NULL)
		tokens = create_add_wildcards_nodes(tokens, tokens->next, cmd, data);
}

static void	add_wildcard_to_gc(t_wsearch wildcard_result, t_minishell *data)
{
	while (wildcard_result.result)
	{
		gc_add(wildcard_result.result->lexeme, data);
		gc_add(wildcard_result.result, data);
		wildcard_result.result = wildcard_result.result->next;
	}
}

static void	clean_current_token(char *token)
{
	while (token && *token)
	{
		if (*token == - '*' || *token == - '?')
			*token = -(*token);
		token++;
	}
}
