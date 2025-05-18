/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:31:08 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/18 23:55:22 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned char	close_and_wait(t_leaf *cmd, t_minishell *data);
static void				secure_dup2(t_leaf *cmd, t_minishell *data);

static int	exec_parenthesized_cmd(t_leaf *cmd, t_minishell *data)
{
	pid_t	pid;

	redirections_in_par_cmd(cmd, data);
	print_debug_cmd(cmd, LEXEME, 10,
		"\ndisplay command after handle redi\n");
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		pid = fork();
		if (pid == 0)
		{
			secure_dup2(cmd, data);
			data->is_subshell = true;
			parsing_exec(tokens_to_str(cmd->tokens->next, data), data);
			exit_minishell(data);
		}
		else if (pid != -1)
			data->last_cmd_pid = pid;
		else
			fork_error(data);
	}
	return (close_and_wait(cmd, data));
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
			if (!data->in_pipe && exec_builtins(argv, false, data))
				return (close_and_wait(cmd, data));
			pid = fork();
			if (pid == 0)
			{
				secure_dup2(cmd, data);
				exec_command(argv, data);
			}
			else if (pid != -1)
				data->last_cmd_pid = pid;
			else
				fork_error(data);
		}
	}
	return (close_and_wait(cmd, data));
}

int	execute_cmd(t_leaf *cmd, t_minishell *data)
{
	if (cmd == NULL)
		return (EXIT_FAILURE);
	if (cmd->parenthesis == true)
		return (exec_parenthesized_cmd(cmd, data));
	return (exec_not_parenthesized_cmd(cmd, data));
}

static unsigned char	close_and_wait(t_leaf *cmd, t_minishell *data)
{
	close_input_output(cmd);
	wait_childs(data);
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		data->exit_code = EXIT_FAILURE;
	return (data->exit_code);
}

static void	secure_dup2(t_leaf *cmd, t_minishell *data)
{
	if (dup2(cmd->fd_input, 0) == -1)
		open_error(data);
	if (dup2(cmd->fd_output, 1) == -1)
		open_error(data);
	if (cmd->fd_input != 0)
		close(cmd->fd_input);
	if (cmd->fd_output != 1)
		close(cmd->fd_output);
}
