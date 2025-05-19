/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_pipe_op.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:32:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/20 01:24:59 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	secure_pipe_dup2(int pipefd[2], t_leaf *cmd, t_minishell *data);

int	exec_parenthesized_cmd_pipe(t_leaf *cmd, t_minishell *data)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		raise_error_category("pipe", data);
	redirections_in_par_cmd(cmd, data);
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		pid = fork();
		if (pid == 0)
		{
			secure_pipe_dup2(pipefd, cmd, data);
			data->is_subshell = true;
			parsing_exec(tokens_to_str(cmd->tokens->next, data), data);
			exit_minishell(data);
		}
		else if (pid == -1)
			raise_error_category("fork", data);
	}
	close(pipefd[1]);
	close_input_output(cmd);
	return (pipefd[0]);
}

int	exec_not_parenthesized_cmd_pipe(t_leaf *cmd, t_minishell *data)
{
	char			**argv;
	int				pipefd[2];
	pid_t			pid;

	if (pipe(pipefd) == -1)
		raise_error_category("pipe", data);
	parse_cmd(cmd, data);
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		argv = tokens_to_argv(cmd->tokens, data);
		if (argv != NULL)
		{
			pid = fork();
			if (pid == 0)
			{
				secure_pipe_dup2(pipefd, cmd, data);
				exec_command(argv, data);
			}
			else if (pid == -1)
				raise_error_category("fork", data);
		}
	}
	close(pipefd[1]);
	close_input_output(cmd);
	return (pipefd[0]);
}

t_leaf	*evaluate_pipe_op(t_ast_node *node, t_minishell *data)
{
	t_leaf	*left;
	t_leaf	*right;

	data->in_pipe = true;
	left = evaluate_ast(node->s_ope_node.left_node, data);
	right = evaluate_ast(node->s_ope_node.right_node, data);
	if (left->parenthesis == true)
		right->fd_input = exec_parenthesized_cmd_pipe(left, data);
	else
		right->fd_input = exec_not_parenthesized_cmd_pipe(left, data);
	return (right);
}

static void	secure_pipe_dup2(int pipefd[2], t_leaf *cmd, t_minishell *data)
{
	close(pipefd[0]);
	if (dup2(pipefd[1], 1) == -1)
	{
		close(pipefd[1]);
		raise_error_category("dup2", data);
	}
	close(pipefd[1]);
	if (dup2(cmd->fd_input, 0) == -1)
		raise_error_category("dup2", data);
	if (dup2(cmd->fd_output, 1) == -1)
		raise_error_category("dup2", data);
	if (cmd->fd_input != 0)
		close(cmd->fd_input);
	if (cmd->fd_output != 1)
		close(cmd->fd_output);
}
