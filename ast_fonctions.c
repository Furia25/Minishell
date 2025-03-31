/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_fonctions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:28:35 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/31 06:59:57 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/incs/ft_printf.h"
#include "libft/incs/get_next_line.h"
void	handle_red_and_del(t_leaf *command_tab, int (*is_redi)());
int	consider_redis(t_leaf *command_tab, t_list *token);
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
t_AST_node	*create_para_node(t_leaf *command_tab)
{
	t_leaf *cmd_tab_in_par;

	cmd_tab_in_par = create_cmd_tab(command_tab->tokens);
	return (create_parent_node(PARENTHESIS, create_ast(cmd_tab_in_par), NULL));
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
	t_leaf *cmd_tab_in_par;

	buff = NULL;
	if (command_tab->ope_after == LINE_CHANGE
		|| command_tab->ope_after == VOID)
	{
		if (command_tab->parenthesis == ON)
			return (create_para_node(command_tab));
		else
			return (create_leaf_node(command_tab));
	}
	latest_logical_op(command_tab, &buff);
	if (buff != NULL)
		return (create_if_found(command_tab, buff));
	latest_pipe_op(command_tab, &buff);
	if (buff != NULL)
		return (create_if_found(command_tab, buff));
	return (NULL);
}

t_leaf	*evaluate_logical_op(t_operator op, t_leaf *left_value, t_leaf	*right_value)
{
	pid_t	pid;

	if (left_value != NULL)
	{
		pid = fork();
		if (pid == 0)
		{
			dup2(left_value->fd_input, 0);
			dup2(left_value->fd_output, 1);
			//execve
		}
	}
	if (op == AND)
	{
		if (left_value->returned_value == 0) // gerer avec $?
			return (right_value);
		return (NULL);
	}
	if (left_value->returned_value != 0)
		return (right_value);
	return (NULL);
}

t_leaf	*evaluate_pipe_op(t_leaf *left_value, t_leaf *right_value)
{
	int	pipefd[2];
	int	fd_input;
	int	fd_output;
	pid_t	pid;

	handle_red_and_del(left_value, consider_redis);
	if (left_value->fd_input != -1 && left_value->fd_output != -1)
	{
		pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			close(pipefd[0]);
			dup2(pipefd[1], 1);
			close(pipefd[1]);
			dup2(left_value->fd_input, 0);
			dup2(left_value->fd_output, 1);
			if (left_value->fd_input != 0)
				close(left_value->fd_input);
			if (left_value->fd_output != 1)
				close(left_value->fd_output);
			ft_printf("%s", get_next_line(0));
			exit(0);
		}
		wait(NULL);
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
	}
	return (right_value);
}

int	execute_cmd(t_leaf *cmd)
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
	handle_red_and_del(cmd, consider_redis);
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		pid = fork();
		if (pid == 0)
		{
			dup2(cmd->fd_input, 0);
			dup2(cmd->fd_output, 1);
			if (cmd->fd_input != 0)
				close(cmd->fd_input);
			if (cmd->fd_output != 1)
				close(cmd->fd_output);
			ft_printf("%s", get_next_line(0));
			//exec
			exit(0);
		}
		wait(NULL);
		return (0);
	}
	return (1);
}


t_leaf	*evaluate_ast(t_AST_node *node)
{
	t_leaf	*left_value;
	t_leaf	*right_value;
	int	pipefd[2];
	pid_t	pid;

	if (node == NULL)
		return (NULL);
	if (node->type == NODE_COMMAND)
		return (node->command);
	if (node->t_ope_node.control_operator == PARENTHESIS)
	{
		pid = fork();
		if (pid == 0)
		{
			execute_cmd(evaluate_ast(node->t_ope_node.left_node));
			exit(0);
		}
		wait(NULL);
		left_value = malloc(sizeof(t_leaf));
		left_value->tokens = NULL;
		left_value->returned_value = 0;
		return(left_value);
	}
	if (node->t_ope_node.control_operator == AND)
	{
		left_value = evaluate_ast(node->t_ope_node.left_node);
		if (execute_cmd(left_value) == 0) // gerer avec $?
		{
			right_value = evaluate_ast(node->t_ope_node.right_node);
			return (right_value);
		}
		return (NULL);
	}
	if (node->t_ope_node.control_operator == OR)
	{
		left_value = evaluate_ast(node->t_ope_node.left_node);
		if (execute_cmd(left_value) != 0) // gerer avec $?
		{
			right_value = evaluate_ast(node->t_ope_node.right_node);
			return (right_value);
		}
		return (NULL);
	}
	if (node->t_ope_node.control_operator == PIPE)
	{
		left_value = evaluate_ast(node->t_ope_node.left_node);
		right_value = evaluate_ast(node->t_ope_node.right_node);
		return (evaluate_pipe_op(left_value, right_value));
	}
	return (NULL);
}

// int	main(void)
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





