/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_leaf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:31:08 by alpayet           #+#    #+#             */
/*   Updated: 2025/06/01 19:29:54 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"
#include "debug.h"

static unsigned char	secure_close_and_wait(t_leaf *cmd, t_minishell *data);
static void				secure_dup2(t_leaf *cmd, t_minishell *data);

static int	exec_parenthesized_cmd(t_leaf *cmd, t_minishell *data)
{
	pid_t	pid;

	parse_par_cmd(cmd, data);
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		pid = s_fork(data);
		if (pid == 0)
		{
			secure_dup2(cmd, data);
			parsing_exec(tokens_to_str(cmd->tokens, data), data);
			exit_minishell(data);
		}
		else if (pid != -1)
			data->last_cmd_pid = pid;
		else
			raise_error_category("fork", data);
	}
	return (secure_close_and_wait(cmd, data));
}

static int	exec_not_parenthesized_cmd(t_leaf *cmd, t_minishell *data)
{
	pid_t			pid;
	char			**argv;

	parse_cmd(cmd, data);
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		argv = tokens_to_argv(cmd->tokens, data);
		if (argv != NULL)
		{
			if (!data->in_pipe && exec_builtins(cmd, argv, false, data))
				return (secure_close_and_wait(cmd, data));
			pid = s_fork(data);
			if (pid == 0)
			{
				secure_dup2(cmd, data);
				exec_command(cmd, argv, data);
			}
			else if (pid != -1)
				data->last_cmd_pid = pid;
			else
				raise_error_category("fork", data);
		}
	}
	return (secure_close_and_wait(cmd, data));
}

int	evaluate_leaf(t_leaf *cmd, t_minishell *data)
{
	if (cmd == NULL)
		return (1);
	if (cmd->parenthesis == true)
		return (exec_parenthesized_cmd(cmd, data));
	return (exec_not_parenthesized_cmd(cmd, data));
}

static unsigned char	secure_close_and_wait(t_leaf *cmd, t_minishell *data)
{
	secure_close_input_output(cmd);
	wait_childs(data);
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		data->exit_code = EXIT_FAILURE;
	return (data->exit_code);
}

static void	secure_dup2(t_leaf *cmd, t_minishell *data)
{
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
