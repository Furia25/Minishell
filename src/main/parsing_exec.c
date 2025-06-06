/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 23:37:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/06/01 19:27:37 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"
#include "environment.h"
#include "debug.h"
#include "redirections.h"

static bool	parsing(char *input, t_minishell *data);
static void	exec(t_minishell *data);

static bool	is_invalid_input(char *input)
{
	size_t	i;

	i = 0;
	while (input[i] != '\0' && ft_strchr(BASIC_WHITESPACE, input[i]) != NULL)
		i++;
	return (input[i] == '\0');
}

void	parsing_exec(char *input, t_minishell *data)
{
	if (input == NULL)
		return ;
	if (parsing(input, data) == false)
		return ;
	exec(data);
	data->line++;
}

static bool	parsing(char *input, t_minishell *data)
{
	t_lst		*tokens;

	if (!data->script_mode)
		setup_signals(SIGCONTEXT_PARENT);
	if (is_invalid_input(input) == true)
		return (false);
	data->last_cmd_pid = -1;
	tokens = NULL;
	data->command_tab = NULL;
	if (create_tokens(&tokens, input, data) == false
		|| have_syntax_errors(tokens, data) == true)
		return (false);
	print_debug_lst(tokens, LEXEME | TYPE, 2,
		"\ndisplay tokens just after creating it\n");
	data->command_tab = create_cmd_tab(tokens, data);
	print_debug_all_cmd(data->command_tab, LEXEME | TYPE, 3,
		"\ndisplay command_tab just after creating it\n");
	if (handle_all_here_doc(data->command_tab, data) == false)
		return (false);
	print_debug_all_cmd(data->command_tab, LEXEME | TYPE, 5,
		"\ndisplay command_tab after handle here doc\n");
	return (true);
}

static void	exec(t_minishell *data)
{
	t_leaf		*final;
	t_ast_node	*top_node_ast;

	data->in_pipe = false;
	if (data->environment_tab)
		free_chartab(data->environment_tab);
	data->environment_tab = make_env(&data->environment);
	if (!data->environment_tab)
		raise_error(data);
	top_node_ast = create_ast(data->command_tab, data);
	print_debug_ast(top_node_ast, 6,
		"\ndisplay AST just after creating it\n");
	final = evaluate_ast(top_node_ast, data);
	evaluate_leaf(final, data);
	rm_all_here_doc_files(data->command_tab);
	gc_free_ast(top_node_ast, data);
	gc_free(data->command_tab, data);
	data->command_tab = NULL;
	gc_clean(data);
}
