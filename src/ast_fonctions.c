/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_fonctions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:28:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/11 19:10:00 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
t_AST_node	*create_ast(t_leaf *command_tab);

t_AST_node	*create_leaf_node(t_leaf *cmd)
{
	t_AST_node	*node;

	node = malloc(sizeof(t_AST_node));
	node->type = NODE_COMMAND;
	node->command = cmd;
	return (node);
}

t_AST_node	*create_parent_node(t_operator ope, t_AST_node *left, t_AST_node *right)
{
	t_AST_node	*node;

	node = malloc(sizeof(t_AST_node));
	node->type = NODE_OPERATOR_CONTROL;
	node->t_ope_node.control_operator = ope;
	node->t_ope_node.left_node = left;
	node->t_ope_node.right_node = right;
	return (node);
}

void	latest_logical_op(t_leaf *cmds, t_leaf **buff)
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

void	latest_pipe_op(t_leaf *cmds, t_leaf **buff)
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

void	latest_simple_red_op(t_leaf *cmds, t_leaf **buff)
{
	t_leaf	*temp;

	temp = cmds;
	while (temp->ope_after != LINE_CHANGE && temp->ope_after != VOID)
	{
		if (temp->ope_after == RED_I || temp->ope_after == RED_O)
			*buff = temp;
		temp++;
	}
}

t_AST_node	*create_if_found(t_leaf *command_tab, t_leaf *buff)
{
	t_operator		op;
	
	op = buff->ope_after;
	buff->ope_after = VOID;
	return (create_parent_node(op, create_ast(command_tab)
			, create_ast(buff + 1)));
}

t_AST_node	*create_ast(t_leaf *command_tab)
{
	t_operator		op;
	t_leaf	*buff;

	buff = NULL;
	if (command_tab->ope_after == LINE_CHANGE
		|| command_tab->ope_after == VOID)
		return (create_leaf_node(command_tab));
	latest_logical_op(command_tab, &buff);
	if (buff != NULL)
		return (create_if_found(command_tab, buff));
	latest_pipe_op(command_tab, &buff);
	if (buff != NULL)
		return (create_if_found(command_tab, buff));
	latest_simple_red_op(command_tab, &buff);
	if (buff != NULL)
		return (create_if_found(command_tab, buff));
	return (NULL);
}

t_leaf	*evaluate_ast(t_AST_node *node)
{
	t_leaf	*left_value;
	t_leaf	*right_value;

	if (node->type == NODE_COMMAND)
		return (node->command);
	left_value = evaluate_ast(node->t_ope_node.left_node);
	right_value = evaluate_ast(node->t_ope_node.right_node);
	if (node->t_ope_node.control_operator == AND)
	{
		if (left_value->returned_value == 0)
			return (right_value);
		return (left_value);
	}
	if (node->t_ope_node.control_operator == OR)
	{
		if (left_value->returned_value != 0)
			return (right_value);
		return (left_value);
	}
	if (node->t_ope_node.control_operator == PIPE)
	{
		right_value->cmd_stdin = left_value->cmd_stdout;
		return (right_value);
	}
	return (NULL);
}

/* int	main(void)
{
	t_leaf	cmd[5];
	t_AST_node	*cmd1_node;
	t_AST_node	*cmd2_node;
	t_AST_node	*cmd3_node;
	t_AST_node	*cmd4_node;
	t_AST_node	*cmd5_node;
	t_AST_node	*pipe_node1;
	t_AST_node	*pipe_node2;
	t_AST_node	*and_node;
	t_AST_node	*or_node;
	t_AST_node	*test_tree;
	t_leaf	*final;
	t_leaf	*final_test;

	cmd[0].cmd_stdin = 'a';
	cmd[0].cmd_stdout = 'b';
	cmd[0].returned_value = 0;
	cmd[0].ope_after = PIPE;

	cmd[1].cmd_stdin = 'c';
	cmd[1].cmd_stdout = 'd';
	cmd[1].returned_value = 1;
	cmd[1].ope_after = AND;

	cmd[2].cmd_stdin = 'e';
	cmd[2].cmd_stdout = 'f';
	cmd[2].returned_value = 2;
	cmd[2].ope_after = PIPE;

	cmd[3].cmd_stdin = 'g';
	cmd[3].cmd_stdout = 'h';
	cmd[3].returned_value = 3;
	cmd[3].ope_after = OR;

	cmd[4].cmd_stdin = 'i';
	cmd[4].cmd_stdout = 'j';
	cmd[4].returned_value = 4;
	cmd[4].ope_after = LINE_CHANGE;

	cmd1_node = create_leaf_node(&cmd[0]);
	cmd2_node = create_leaf_node(&cmd[1]);
	cmd3_node = create_leaf_node(&cmd[2]);
	cmd4_node = create_leaf_node(&cmd[3]);
	cmd5_node = create_leaf_node(&cmd[4]);
	pipe_node1 = create_parent_node(PIPE, cmd1_node, cmd2_node);
	pipe_node2 = create_parent_node(PIPE, cmd3_node, cmd4_node);
	and_node = create_parent_node(AND, pipe_node1, pipe_node2);
	or_node = create_parent_node(OR, and_node, cmd5_node);
	test_tree = create_ast(cmd);
	final = evaluate_ast(or_node);
	final_test = evaluate_ast(test_tree);
	printf("stdin: %c\nstdout: %c\nvalue: %d\n\n", final->cmd_stdin,   final->cmd_stdout, final->returned_value);
	printf("stdin: %c\nstdout: %c\nvalue: %d\n\n", final_test->cmd_stdin,   final_test->cmd_stdout, final_test->returned_value);
	// printf("ope_after: %d\n", cmd[0].ope_after);
	// printf("ope_after: %d\n", cmd[1].ope_after);
	// printf("ope_after: %d\n", cmd[2].ope_after);

}*/





