/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:07:28 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/26 02:05:14 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
void	handle_subshell_in_cmd(t_leaf *command_tab, t_minishell *data);
void	fusion_quote_token(t_lst *tokens, t_minishell *data);
void	handle_reds_and_del(t_leaf *command_tab, t_minishell *data);

t_AST_node	*create_leaf_node(t_leaf *cmd, t_minishell *data)
{
	t_AST_node	*node;

	node = malloc(sizeof(t_AST_node));
	check_malloc(node, data);
	node->type = NODE_COMMAND;
	node->command = cmd;
	return (node);
}

t_AST_node	*create_parent_node(t_lexeme_type ope, t_AST_node *left, t_AST_node *right, t_minishell *data)
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

t_AST_node	*create_if_found(t_leaf *command_tab, t_leaf *buff, t_minishell *data)
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

int	execute_cmd(t_leaf *cmd, t_minishell *data)
{
	pid_t	pid;
	int returned_value;

	if (cmd == NULL)
		return (1);
	if (cmd->tokens == NULL)
	{
		returned_value = cmd->returned_value;
		free(cmd);
		return (returned_value);
	}
	handle_subshell_in_cmd(cmd, data);
	print_debug_lst(cmd->tokens, LEXEME_AND_TYPE, 6,
		"\ndisplay command->tokens after handle ev_expension and subshell\n");
	fusion_quote_token(cmd->tokens, data);
	print_debug_lst(cmd->tokens, ONLY_LEXEME, 7,
		"\ndisplay command->tokens after handle fusion quotes\n");
	pid = fork();
	if (pid == 0)
	{
		if (cmd->parenthesis == false)
		{
			handle_reds_and_del(cmd, data);
			if (cmd->fd_input == -1 || cmd->fd_output == -1)
				exit(1);
			print_debug_all_cmd(cmd, ONLY_LEXEME, 8,
				"\ndisplay command after handle redi\n");
			dup2(cmd->fd_input, 0);
			dup2(cmd->fd_output, 1);
			if (cmd->fd_input != 0)
				close(cmd->fd_input);
			if (cmd->fd_output != 1)
				close(cmd->fd_output);
			ft_printf("%s", get_next_line(0));
			exit(0);
		}
		else
		{}	//execve minishell
	}
	wait(NULL);
	return (1);
}

t_leaf	*evaluate_logical_op(t_AST_node *node, t_minishell *data)
{
	t_leaf	*left_value;
	t_leaf	*right_value;

	if (node->t_ope_node.control_operator == AND)
	{
		left_value = evaluate_ast(node->t_ope_node.left_node, data);
		if (execute_cmd(left_value, data) == 0) // gerer avec $?
		{
			right_value = evaluate_ast(node->t_ope_node.right_node, data);
			return (right_value);
		}
		return (NULL);
	}
	left_value = evaluate_ast(node->t_ope_node.left_node, data);
	if (execute_cmd(left_value, data) != 0) // gerer avec $?
	{
		right_value = evaluate_ast(node->t_ope_node.right_node, data);
		return (right_value);
	}
	return (NULL);
}

t_leaf	*evaluate_pipe_op(t_AST_node *node, t_minishell *data)
{
	t_leaf	*left_value;
	t_leaf	*right_value;
	int		pipefd[2];
	pid_t		pid;

	left_value = evaluate_ast(node->t_ope_node.left_node, data);
	right_value = evaluate_ast(node->t_ope_node.right_node, data);
	handle_subshell_in_cmd(left_value, data);
	print_debug_lst(left_value->tokens, LEXEME_AND_TYPE, 6,
		"\ndisplay command->tokens after handle ev_expension and subshell\n");
	fusion_quote_token(left_value->tokens, data);
	print_debug_lst(left_value->tokens, ONLY_LEXEME, 7,
		"\ndisplay command->tokens after handle fusion quotes\n");
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		if (left_value->parenthesis == false)
		{
			handle_reds_and_del(left_value, data);
			if (left_value->fd_input == -1 || left_value->fd_output == -1)
				exit(1);
			print_debug_all_cmd(left_value, ONLY_LEXEME, 8,
				"\ndisplay command after handle redi\n");
			dup2(left_value->fd_input, 0);
			dup2(left_value->fd_output, 1);
			if (left_value->fd_input != 0)
				close(left_value->fd_input);
			if (left_value->fd_output != 1)
				close(left_value->fd_output);
			ft_printf("%s", get_next_line(0));
			exit(0);
		}
		else
		{}	//execve minishell
		close(pipefd[1]);
		right_value->fd_input = pipefd[0];
		if (left_value->fd_input != 0)
			close(left_value->fd_input);
		if (left_value->fd_output != 1)
			close(left_value->fd_output);
	}
	close(pipefd[1]);
	right_value->fd_input = pipefd[0];
	return (right_value);
}

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

/*  int	main(void)
// {
// 	t_leaf	cmd[5];
// 	t_AST_node	*cmd1_node;
// 	t_AST_node	*cmd2_node;
// 	t_AST_node	*cmd3_node;
// 	t_AST_node	*cmd4_node;
// 	t_AST_node	*cmd5_node;
// 	t_AST_node	*pipe_node1;
// 	t_AST_node	*pipe_node2;
// 	t_AST_node	*and_node;
// 	t_AST_node	*or_node;
// 	t_AST_node	*test_tree;
// 	t_leaf	*final;
// 	t_leaf	*final_test;

// 	cmd[0].fd_input = 'a';
// 	cmd[0].fd_output = 'b';
// 	cmd[0].returned_value = 0;
// 	cmd[0].ope_after = PIPE;

// 	cmd[1].fd_input = 'c';
// 	cmd[1].fd_output = 'd';
// 	cmd[1].returned_value = 1;
// 	cmd[1].ope_after = AND;

// 	cmd[2].fd_input = 'e';
// 	cmd[2].fd_output = 'f';
// 	cmd[2].returned_value = 2;
// 	cmd[2].ope_after = PIPE;

// 	cmd[3].fd_input = 'g';
// 	cmd[3].fd_output = 'h';
// 	cmd[3].returned_value = 3;
// 	cmd[3].ope_after = OR;

// 	cmd[4].fd_input = 'i';
// 	cmd[4].fd_output = 'j';
// 	cmd[4].returned_value = 4;
// 	cmd[4].ope_after = LINE_CHANGE;

// 	cmd1_node = create_leaf_node(&cmd[0]);
// 	cmd2_node = create_leaf_node(&cmd[1]);
// 	cmd3_node = create_leaf_node(&cmd[2]);
// 	cmd4_node = create_leaf_node(&cmd[3]);
// 	cmd5_node = create_leaf_node(&cmd[4]);
// 	pipe_node1 = create_parent_node(PIPE, cmd1_node, cmd2_node);
// 	pipe_node2 = create_parent_node(PIPE, cmd3_node, cmd4_node);
// 	and_node = create_parent_node(AND, pipe_node1, pipe_node2);
// 	or_node = create_parent_node(OR, and_node, cmd5_node);
// 	test_tree = create_ast(cmd);
// 	final = evaluate_ast(or_node);
// 	final_test = evaluate_ast(test_tree);
// 	printf("stdin: %c\nstdout: %c\nvalue: %d\n\n", final->fd_input,   final->fd_output, final->returned_value);
// 	printf("stdin: %c\nstdout: %c\nvalue: %d\n\n", final_test->fd_input,   final_test->fd_output, final_test->returned_value);
// 	// printf("ope_after: %d\n", cmd[0].ope_after);
// 	// printf("ope_after: %d\n", cmd[1].ope_after);
// 	// printf("ope_after: %d\n", cmd[2].ope_after);

// }
 */