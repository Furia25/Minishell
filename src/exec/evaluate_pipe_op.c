/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_pipe_op.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:32:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/15 19:28:10 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
void	parse_cmd(t_leaf *cmd, t_minishell *data);
char	**tokens_to_argv(t_lst *tokens, t_minishell *data);
size_t tab_size(char **tab);

t_leaf	*evaluate_pipe_op(t_AST_node *node, t_minishell *data)
{
	t_leaf	*left_value;
	t_leaf	*right_value;
	char		**argv;
	int		pipefd[2];
	pid_t		pid;

	left_value = evaluate_ast(node->t_ope_node.left_node, data);
	right_value = evaluate_ast(node->t_ope_node.right_node, data);
	pipe(pipefd);
	parse_cmd(left_value, data);
	if (left_value->fd_input != -1 && left_value->fd_output != -1)
	{
		argv = tokens_to_argv(left_value->tokens, data);
		print_debug_argv(argv, 11,
		"\ndisplay argv after creating it\n");
		if (argv != NULL)
		{
			pid = fork();
			if (pid == 0)
			{
				close(pipefd[0]);
				dup2(pipefd[1], 1);
				close(pipefd[1]);
				dup2(left_value->fd_input, 0);
				if (left_value->fd_input != 0)
					close(left_value->fd_input);
				dup2(left_value->fd_output, 1);
				if (left_value->fd_output != 1)
					close(left_value->fd_output);
				if (left_value->parenthesis == false)
				{
					t_builtin_type type = get_builtin(argv[0]);
					if (type != BUILTIN_TYPE_NOTBUILTIN)
					{
						if (!try_builtin(type, tab_size(argv), argv, data))
							exit_minishell(data);
					}
					else
					{
						char *command_path = find_command(argv[0], data);
						if (!command_path)
							command_notfound(argv[0], data);
						execve(command_path, argv, data->environment_tab);
						free(command_path);
					}
					exit_minishell(data);
				}
				else
				{
					data->is_subshell = true;
					parsing_exec("ls", data);
					exit_minishell(data);
				}
			}
		}
	}
	close(pipefd[1]);
	if (left_value->fd_input != 0 && left_value->fd_input != -1)
		close(left_value->fd_input);
	if (left_value->fd_output != 1 && left_value->fd_output != -1)
		close(left_value->fd_output);
	right_value->fd_input = pipefd[0];
	return (right_value);
}
