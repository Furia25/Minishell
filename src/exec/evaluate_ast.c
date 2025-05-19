/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:27:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/20 01:24:59 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_leaf	*evaluate_logical_op(t_ast_node *node, t_minishell *data);
t_leaf	*evaluate_pipe_op(t_ast_node *node, t_minishell *data);

t_leaf	*evaluate_ast(t_ast_node *node, t_minishell *data)
{
	if (node->type == NODE_COMMAND)
		return (node->command);
	if (node->s_ope_node.control_operator == AND)
		return (evaluate_logical_op(node, data));
	if (node->s_ope_node.control_operator == OR)
		return (evaluate_logical_op(node, data));
	if (node->s_ope_node.control_operator == PIPE)
		return (evaluate_pipe_op(node, data));
	return (NULL);
}
