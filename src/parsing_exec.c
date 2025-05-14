/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 23:37:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/14 22:32:35 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	create_tokens(t_lst **tokens, char *input, t_minishell *data);
int	check_syntax_errors(t_lst *tokens, t_minishell *data);
t_leaf	*create_cmd_tab(t_lst *tokens, t_minishell *data);
void	handle_all_here_doc(t_leaf *command_tab, t_minishell *data);
void	rm_all_here_doc_files(t_leaf *command_tab);

void	parsing_exec(char *input, t_minishell *data)
{
	t_lst	*tokens;
	t_leaf *command_tab;
	t_AST_node	*top_node_ast;
	t_leaf	*final;

	data->last_cmd_pid = -1;
	tokens = NULL;
	if (create_tokens(&tokens, input, data) == EXIT_FAILURE
		|| check_syntax_errors(tokens, data) == EXIT_FAILURE)
		return ;
	print_debug_lst(tokens, LEXEME | TYPE, 2,
		 "\ndisplay tokens just after creating it\n");
	command_tab = create_cmd_tab(tokens, data);
	print_debug_all_cmd(command_tab, LEXEME | TYPE, 3,
		 "\ndisplay command_tab just after creating it\n");
	handle_all_here_doc(command_tab, data);
	if (data->exit_code == 1) // <- C'est chelou ? 
		return ;
	print_debug_all_cmd(command_tab, LEXEME | TYPE, 5
		,"\ndisplay command_tab after handle here doc\n");
	if (data->environment_tab)
		free_chartab(data->environment_tab);
	data->environment_tab = make_env(&data->environment);
	if (!data->environment_tab)
		malloc_error(data);
	top_node_ast = create_ast(command_tab, data);
	print_debug_ast(top_node_ast, 6, 
		"\ndisplay AST just after creating it\n");
	final = evaluate_ast(top_node_ast, data);
	execute_cmd(final, data);
	rm_all_here_doc_files(command_tab);
	gc_free_ast(top_node_ast, data);
	gc_free(command_tab, data);
}

