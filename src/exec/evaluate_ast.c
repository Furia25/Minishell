/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:27:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/16 18:36:35 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
t_leaf	*evaluate_logical_op(t_AST_node *node, t_minishell *data);
t_leaf	*evaluate_pipe_op(t_AST_node *node, t_minishell *data);

t_leaf	*evaluate_ast(t_AST_node *node, t_minishell *data)
{
	if (node->type == NODE_COMMAND)
		return (node->command);
	if (node->t_ope_node.control_operator == AND)
		return (evaluate_logical_op(node, data));
	if (node->t_ope_node.control_operator == OR)
		return (evaluate_logical_op(node, data));
	if (node->t_ope_node.control_operator == PIPE)
		return (evaluate_pipe_op(node, data));
	return (NULL);
}
