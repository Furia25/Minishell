/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:17:52 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 05:28:44 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
t_AST_node	*create_leaf_node(t_leaf *cmd, t_minishell *data);
t_AST_node	*create_parent_node(t_lexeme_type ope, t_AST_node *left, t_AST_node *right, t_minishell *data);

static void	latest_logical_op(t_leaf *cmds, t_leaf **buff)
{
	t_leaf	*temp;

	temp = cmds;
	while (temp->ope_after != LINE_CHANGE && temp->ope_after != VOID)
	{
		if (temp->ope_after == AND || temp->ope_after == OR)
			*buff = temp;
		temp++;
	}
}

static void	latest_pipe_op(t_leaf *cmds, t_leaf **buff)
{
	t_leaf	*temp;

	temp = cmds;
	while (temp->ope_after != LINE_CHANGE && temp->ope_after != VOID)
	{
		if (temp->ope_after == PIPE)
			*buff = temp;
		temp++;
	}
}

static t_AST_node	*create_if_found(t_leaf *command_tab, t_leaf *buff, t_minishell *data)
{
	t_lexeme_type	op;

	op = buff->ope_after;
	buff->ope_after = VOID;
	return (create_parent_node(op, create_ast(command_tab, data)
			, create_ast(buff + 1, data), data));
}

t_AST_node	*create_ast(t_leaf *command_tab, t_minishell *data)
{
	t_leaf	*buff;

	buff = NULL;
	if (command_tab->ope_after == LINE_CHANGE
		|| command_tab->ope_after == VOID)
		return (create_leaf_node(command_tab, data));
	latest_logical_op(command_tab, &buff);
	if (buff != NULL)
		return (create_if_found(command_tab, buff, data));
	latest_pipe_op(command_tab, &buff);
	if (buff != NULL)
		return (create_if_found(command_tab, buff, data));
	return (NULL);
}
