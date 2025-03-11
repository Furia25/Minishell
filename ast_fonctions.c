/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_fonctions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:28:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/11 15:12:40 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

t_AST_node	*create_leaf_node(t_leaf *cmd)
{
	t_AST_node *node;

	node = malloc(sizeof(t_AST_node));
	node->type = NODE_COMMAND;
	node->command = cmd;
	return (node);
}

t_AST_node	*create_parent_node(int control_ope, t_AST_node *left, t_AST_node *right)
{
	t_AST_node *node;

	node = malloc(sizeof(t_AST_node));
	node->type = NODE_OPERATOR_CONTROL;
	node->t_ope_node.control_operator = control_ope;
	node->t_ope_node.left_node = left;
	node->t_ope_node.right_node = right;
	return (node);
}
t_AST_node	*create_tree(t_leaf	*commands_list)
{


}

t_leaf *calculate_ast(t_AST_node *node)
{
	t_leaf *left_value;
	t_leaf *right_value;

	if (node->type == NODE_COMMAND)
		return (node->command);
	left_value = calculate_ast(node->t_ope_node.left_node);
	right_value = calculate_ast(node->t_ope_node.right_node);
	if (node->t_ope_node.control_operator == AND)
	{
		if (left_value->returned_value == 0)
			return(right_value);
		return(left_value);
	}
	if (node->t_ope_node.control_operator == OR)
	{
		if (left_value->returned_value != 0)
			return(right_value);
		return(left_value);
	}
	if (node->t_ope_node.control_operator == PIPE)
	{
		right_value->cmd_stdin = left_value->cmd_stdout;
		return(right_value);
	}
	return (NULL);

}

int	main(void)
{
	t_leaf	cmd1;
	t_AST_node	*cmd1_node;
	t_leaf	cmd2;
	t_AST_node	*cmd2_node;
	t_leaf	cmd3;
	t_AST_node	*cmd3_node;
	t_AST_node	*op_pipe;
	t_AST_node	*op_et;
	t_leaf	*final;

	cmd1.cmd_stdin = 'a';
	cmd1.cmd_stdout = 'b';
	cmd1.returned_value = 0;

	cmd2.cmd_stdin = 'c';
	cmd2.cmd_stdout = 'd';
	cmd2.returned_value = 1;

	cmd3.cmd_stdin = 'e';
	cmd3.cmd_stdout = 'f';
	cmd3.returned_value = 2;

	cmd1_node = create_leaf_node(&cmd1);
	cmd2_node = create_leaf_node(&cmd2);
	cmd3_node = create_leaf_node(&cmd3);
	op_pipe = create_parent_node(PIPE, cmd1_node, cmd2_node);
	op_et = create_parent_node(OR, op_pipe, cmd3_node);
	final = calculate_ast(op_et);
	printf("stdin: %c\nstdout: %c\nvalue: %d\n", final->cmd_stdin,   final->cmd_stdout, final->returned_value);
}





