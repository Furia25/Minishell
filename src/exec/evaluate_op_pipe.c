/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_op_pipe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:53:49 by alpayet           #+#    #+#             */
/*   Updated: 2025/06/01 18:58:10 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"
#include "debug.h"

static void	secure_pipe_dup2(int pipefd[2], t_leaf *cmd, t_minishell *data);

int	exec_parenthesized_cmd_pipe(t_leaf *cmd, t_minishell *data)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		raise_error_category("pipe", data);
	parse_par_cmd(cmd, data);
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		pid = s_fork(data);
		if (pid == 0)
		{
			secure_pipe_dup2(pipefd, cmd, data);
			parsing_exec(tokens_to_str(cmd->tokens, data), data);
			exit_minishell(data);
		}
		else if (pid == -1)
			raise_error_category("fork", data);
	}
	secure_close(pipefd[1]);
	secure_close_input_output(cmd);
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
			pid = s_fork(data);
			if (pid == 0)
			{
				secure_pipe_dup2(pipefd, cmd, data);
				exec_command(cmd, argv, data);
			}
			else if (pid == -1)
				raise_error_category("fork", data);
		}
	}
	secure_close(pipefd[1]);
	secure_close_input_output(cmd);
	return (pipefd[0]);
}

t_leaf	*evaluate_op_pipe(t_ast_node *node, t_minishell *data)
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
	secure_close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		secure_close(pipefd[1]);
		raise_error_category("dup2", data);
	}
	secure_close(pipefd[1]);
	if (dup2(cmd->fd_input, STDIN_FILENO) == -1)
		raise_error_category("dup2", data);
	if (dup2(cmd->fd_output, STDOUT_FILENO) == -1)
		raise_error_category("dup2", data);
	if (cmd->fd_input != STDIN_FILENO)
	{
		secure_close(cmd->fd_input);
		cmd->fd_input = STDIN_FILENO;
	}
	if (cmd->fd_output != STDOUT_FILENO)
	{
		secure_close(cmd->fd_output);
		cmd->fd_output = STDOUT_FILENO;
	}
}
