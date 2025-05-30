/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:27:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/30 16:54:15 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_leaf	*evaluate_op_logical(t_ast_node *node, t_minishell *data);
t_leaf	*evaluate_op_pipe(t_ast_node *node, t_minishell *data);

t_leaf	*evaluate_ast(t_ast_node *node, t_minishell *data)
{
	if (node->type == NODE_COMMAND)
		return (node->command);
	if (node->s_ope_node.control_operator == AND)
		return (evaluate_op_logical(node, data));
	if (node->s_ope_node.control_operator == OR)
		return (evaluate_op_logical(node, data));
	if (node->s_ope_node.control_operator == PIPE)
		return (evaluate_op_pipe(node, data));
	return (NULL);
}
