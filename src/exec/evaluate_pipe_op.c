/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_pipe_op.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:32:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/29 18:34:43 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
void	ev_subshell_in_cmd(t_leaf *command_tab, t_minishell *data);
void	fusion_quote_token(t_lst *tokens, t_minishell *data);
void	handle_reds_and_del(t_leaf *command_tab, t_minishell *data);
char	**tokens_to_argv(t_lst *tokens, t_minishell *data);

t_leaf	*evaluate_pipe_op(t_AST_node *node, t_minishell *data)
{
	t_leaf	*left_value;
	t_leaf	*right_value;
	int		pipefd[2];
	pid_t		pid;

	left_value = evaluate_ast(node->t_ope_node.left_node, data);
	right_value = evaluate_ast(node->t_ope_node.right_node, data);
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		if (left_value->parenthesis == false)
		{
			print_debug_argv(tokens_to_argv(left_value->tokens, data), 9,
			"\ndisplay argv after creating it\n");
			dup2(left_value->fd_input, 0);
			dup2(left_value->fd_output, 1);
			if (left_value->fd_input != 0)
				close(left_value->fd_input);
			if (left_value->fd_output != 1)
				close(left_value->fd_output);
			ft_printf("%s", get_next_line(0));
			exit(0);
		}
		else
		{}	//execve minishell
	}
	close(pipefd[1]);
	if (left_value->fd_input != 0)
		close(left_value->fd_input);
	if (left_value->fd_output != 1)
		close(left_value->fd_output);
	right_value->fd_input = pipefd[0];
	return (right_value);
}
