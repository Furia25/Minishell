/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node_ast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:15:51 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/18 23:49:31 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_AST_node	*create_leaf_node(t_leaf *cmd, t_minishell *data)
{
	t_AST_node	*node;

	node = malloc(sizeof(t_AST_node));
	check_malloc(node, data);
	node->type = NODE_COMMAND;
	node->command = cmd;
	return (node);
}

t_AST_node	*create_parent_node(t_lexeme_type ope, t_AST_node *left,
				t_AST_node *right, t_minishell *data)
{
	t_AST_node	*node;

	node = malloc(sizeof(t_AST_node));
	check_malloc(node, data);
	node->type = NODE_OPERATOR_CONTROL;
	node->t_ope_node.control_operator = ope;
	node->t_ope_node.left_node = left;
	node->t_ope_node.right_node = right;
	return (node);
}
