/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_pipe_op.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:32:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/17 13:48:04 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	secure_pipe_dup2(int pipefd[2], t_leaf *cmd, t_minishell *data);

int exec_parenthesized_cmd_pipe(t_leaf *cmd, t_minishell *data)
{
	int		pipefd[2];
	pid_t		pid;

	if (pipe(pipefd) == -1)
		pipe_error(data);
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
			fork_error(data);
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
		pipe_error(data);
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
				fork_error(data);
		}
	}
	close(pipefd[1]);
	close_input_output(cmd);
	return (pipefd[0]);
}

t_leaf	*evaluate_pipe_op(t_AST_node *node, t_minishell *data)
{
	t_leaf	*left_value;
	t_leaf	*right_value;

	data->in_pipe = true;
	left_value = evaluate_ast(node->t_ope_node.left_node, data);
	right_value = evaluate_ast(node->t_ope_node.right_node, data);
	if (left_value->parenthesis == true)
		right_value->fd_input = exec_parenthesized_cmd_pipe(left_value, data);
	else
		right_value->fd_input = exec_not_parenthesized_cmd_pipe(left_value, data);
	return (right_value);
}

static void	secure_pipe_dup2(int pipefd[2], t_leaf *cmd, t_minishell *data)
{
	close(pipefd[0]);
	if (dup2(pipefd[1], 1) == -1)
	{
		close(pipefd[1]);
		close_input_output(cmd);
		open_error(data);
	}
	close(pipefd[1]);
	if (dup2(cmd->fd_input, 0) == -1)
	{
		close_input_output(cmd);
		open_error(data);
	}
	if (cmd->fd_input != 0)
		close(cmd->fd_input);
	if (dup2(cmd->fd_output, 1) == -1)
	{
		close_input_output(cmd);
		open_error(data);
	}
	if (cmd->fd_output != 1)
		close(cmd->fd_output);
}
