/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:31:08 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/09 19:37:40 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
void	ev_subshell_in_cmd(t_leaf *command_tab, t_minishell *data);
void	fusion_quote_token(t_lst *tokens, t_minishell *data);
void	handle_reds_and_del(t_leaf *command_tab, t_minishell *data);
char	**tokens_to_argv(t_lst *tokens, t_minishell *data);

int	execute_cmd(t_leaf *cmd, t_minishell *data)
{
	pid_t	pid;
	char	**argv;

	if (cmd == NULL)
		return (EXIT_FAILURE);
	ev_subshell_in_cmd(cmd, data);
	print_debug_lst(cmd->tokens, LEXEME | TYPE, 6,
		"\ndisplay command->tokens after handle ev_expension and subshell\n");
	fusion_quote_token(cmd->tokens, data);
	print_debug_lst(cmd->tokens, LEXEME, 7,
		"\ndisplay command->tokens after handle fusion quotes\n");
	handle_reds_and_del(cmd, data);
	print_debug_cmd(cmd, LEXEME, 8,
		"\ndisplay command after handle redi\n");
	if (cmd->fd_input != -1 && cmd->fd_output != -1)
	{
		argv = tokens_to_argv(cmd->tokens, data);
		print_debug_argv(argv, 9,
			"\ndisplay argv after creating it\n");
		/*BUILTIN HANDLER THIS IS JUST A TEST*/
		t_builtin_type type = get_builtin(argv[0]);
		// ft_putnbr_fd(type, 2);
		if (type != BUILTIN_TYPE_NOTBUILTIN)
		{
			if (!try_builtin(type, 1, argv, data))
				exit_minishell(data);
			return EXIT_SUCCESS;
		}
		else if (argv != NULL)
		{
			pid = fork();
			if (pid == 0)
			{
				if (cmd->parenthesis == false)
				{
					data->in_child = true;
					dup2(cmd->fd_input, 0);
					dup2(cmd->fd_output, 1);
					if (cmd->fd_input != 0)
						close(cmd->fd_input);
					if (cmd->fd_output != 1)
						close(cmd->fd_output);
					char *command_path = find_command(argv[0], data);
					if (!command_path)
						ft_putstr_fd("LA COMMANDE NEST PAS TROUVE GROS CACA\n", 2);
					execve(command_path, argv, make_env(&data->environment));
					free(command_path);
					exit(0);
				}
				else
				{}	//execve minishell
			}
		}
	}
	wait(NULL);
	if (cmd->fd_input != 0 && cmd->fd_input != -1)
		close(cmd->fd_input);
	if (cmd->fd_output != 1 && cmd->fd_output != -1)
		close(cmd->fd_output);
	if (cmd->fd_input == -1 || cmd->fd_output == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
