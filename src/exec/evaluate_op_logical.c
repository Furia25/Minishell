/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_op_logical.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:53:58 by alpayet           #+#    #+#             */
/*   Updated: 2025/06/01 19:27:37 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_leaf	*evaluate_op_logical(t_ast_node *node, t_minishell *data)
{
	t_leaf	*left_value;
	t_leaf	*right_value;
	static int	final_state;

	if (node->s_ope_node.control_operator == AND)
	{
		left_value = evaluate_ast(node->s_ope_node.left_node, data);
		if (left_value != NULL)
			final_state = evaluate_leaf(left_value, data);
		if (final_state == 0)
		{
			right_value = evaluate_ast(node->s_ope_node.right_node, data);
			return (right_value);
		}
		return (NULL);
	}
	left_value = evaluate_ast(node->s_ope_node.left_node, data);
	if (left_value != NULL)
		final_state = evaluate_leaf(left_value, data);
	if (final_state != 0)
	{
		right_value = evaluate_ast(node->s_ope_node.right_node, data);
		return (right_value);
	}
	return (NULL);
}
