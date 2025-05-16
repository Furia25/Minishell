/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:31:08 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/16 15:53:23 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_childs(t_minishell *data)
{
	int	status;

	if (data->last_cmd_pid == -1)
	{
		while (wait(NULL) != -1);
		return ;
	}
	waitpid(data->last_cmd_pid, &status, 0);
	data->exit_code = (status >> 8) & 0xFF;
	while (wait(NULL) != -1);
}

static int	exec_parenthesized_cmd(t_leaf *cmd, t_minishell *data)
{
	pid_t	pid;
	char	*command_path;

	redirections_in_par_cmd(cmd, data);
	print_debug_cmd(cmd, LEXEME, 10,
		"\ndisplay command after handle redi\n");
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
			data->is_subshell = true;
			parsing_exec(tokens_to_str(cmd->tokens->next, data), data);
			exit_minishell(data);
		}
		else if (pid != -1)
			data->last_cmd_pid = pid;
		else
			ft_putstr_fd("Error", 2);
	}
	if (cmd->fd_input != 0 && cmd->fd_input != -1)
		close(cmd->fd_input);
	if (cmd->fd_output != 1 && cmd->fd_output != -1)
		close(cmd->fd_output);
	wait_childs(data);
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		data->exit_code = EXIT_FAILURE;
	return (data->exit_code);
}


static int	exec_not_parenthesized_cmd(t_leaf *cmd, t_minishell *data)
{
	pid_t	pid;
	char	**argv;
	char	*command_path;

	parse_cmd(cmd, data);
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		argv = tokens_to_argv(cmd->tokens, data);
		if (argv != NULL)
		{
			/*BUILTIN HANDLER THIS IS JUST A TEST*/
			t_builtin_type type = get_builtin(argv[0]);
			// ft_putnbr_fd(type, 2);
			if (type != BUILTIN_TYPE_NOTBUILTIN)
			{
				if (!try_builtin(type, tab_size(argv), argv, data))
					exit_minishell(data);
			}
			else
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
					command_path = find_command(argv[0], data);
					if (!command_path)
						command_notfound(argv[0], data);
					execve(command_path, argv, data->environment_tab);
					data->exit_code = EXIT_FAILURE;
					free(command_path);
					exit_minishell(data);
				}
				else if (pid != -1)
					data->last_cmd_pid = pid;
				else
					ft_putstr_fd("Error", 2);
			}
		}
	}
	if (cmd->fd_input != 0 && cmd->fd_input != -1)
		close(cmd->fd_input);
	if (cmd->fd_output != 1 && cmd->fd_output != -1)
		close(cmd->fd_output);
	wait_childs(data);
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		data->exit_code = EXIT_FAILURE;
	return (data->exit_code);
}

int	execute_cmd(t_leaf *cmd, t_minishell *data)
{	
	if (cmd == NULL)
		return (EXIT_FAILURE);
	if (cmd->parenthesis == true)
		return (exec_parenthesized_cmd(cmd, data));
	return (exec_not_parenthesized_cmd(cmd, data));
}
