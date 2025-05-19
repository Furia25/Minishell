/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node_ast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:15:51 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/20 01:24:59 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_leaf_node(t_leaf *cmd, t_minishell *data)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	check_malloc(node, data);
	node->type = NODE_COMMAND;
	node->command = cmd;
	return (node);
}

t_ast_node	*create_parent_node(t_lexeme_type ope, t_ast_node *left,
				t_ast_node *right, t_minishell *data)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	check_malloc(node, data);
	node->type = NODE_OPERATOR_CONTROL;
	node->s_ope_node.control_operator = ope;
	node->s_ope_node.left_node = left;
	node->s_ope_node.right_node = right;
	return (node);
}
