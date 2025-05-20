/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_logical_op.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:29:52 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/20 14:43:34 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_leaf	*evaluate_logical_op(t_ast_node *node, t_minishell *data)
{
	t_leaf	*left_value;
	t_leaf	*right_value;

	if (node->s_ope_node.control_operator == AND)
	{
		left_value = evaluate_ast(node->s_ope_node.left_node, data);
		if (execute_cmd(left_value, data) == 0)
		{
			right_value = evaluate_ast(node->s_ope_node.right_node, data);
			return (right_value);
		}
		return (NULL);
	}
	left_value = evaluate_ast(node->s_ope_node.left_node, data);
	if (execute_cmd(left_value, data) != 0)
	{
		right_value = evaluate_ast(node->s_ope_node.right_node, data);
		return (right_value);
	}
	return (NULL);
}
