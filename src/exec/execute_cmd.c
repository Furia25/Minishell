/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:31:08 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/15 15:35:23 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
void	parse_cmd(t_leaf *cmd, t_minishell *data);
char	**tokens_to_argv(t_lst *tokens, t_minishell *data);
size_t	tab_size(char **tab);


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

int	execute_cmd(t_leaf *cmd, t_minishell *data)
{
	pid_t	pid;
	char	**argv;
	char	*command_path;

	if (cmd == NULL)
		return (EXIT_FAILURE);
	parse_cmd(cmd, data);
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		argv = tokens_to_argv(cmd->tokens, data);
		print_debug_argv(argv, 11,
			"\ndisplay argv after creating it\n");
		if (argv == NULL)
			ft_printf("Error");
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
				if (cmd->parenthesis == false)
				{
					command_path = find_command(argv[0], data);
					if (!command_path)
						command_notfound(argv[0], data);	
					execve(command_path, argv, data->environment_tab);
					data->exit_code = EXIT_FAILURE;
					exit_minishell(data);
					free(command_path);
				}
				else
				{
					data->is_subshell = true;
					parsing_exec("ls", data);
					exit_minishell(data);
				}
			}
			else if (pid != -1)
				data->last_cmd_pid = pid;
			else
				ft_putstr_fd("Error", 2);
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
