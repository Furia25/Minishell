/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_in_par_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:54:39 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/30 02:33:10 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcards.h"

t_lst	*create_add_wildcards_nodes(t_lst *prev, t_lst *current,
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

void	wildcards_in_par_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst	*tokens;

	tokens = NULL;
	if (is_parenthesized(cmd->tokens) == false)
		tokens = create_add_wildcards_nodes(NULL, cmd->tokens, cmd, data);
	while (tokens != NULL && tokens->next != NULL)
	{
		if (is_parenthesized(tokens->next) == false)
			tokens = create_add_wildcards_nodes(tokens, tokens->next, cmd, data);
		else
			tokens = tokens->next;
	}
}
