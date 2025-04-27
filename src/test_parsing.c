/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 23:37:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 01:01:00 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	create_tokens(t_lst **tokens, char *input, t_minishell *data);
int	check_syntax_errors(t_lst *tokens, t_minishell *data);
t_leaf	*create_cmd_tab(t_lst *tokens, t_minishell *data);
void	handle_all_here_doc(t_leaf *command_tab, t_minishell *data);
void	rm_here_doc_files(t_leaf *command_tab);

int	main(int argc, char **argv, char **envp)
{
	char *input = "echo 'n'$a'n'";
	t_minishell	data;
	t_lst	*tokens;
	t_leaf *command_tab;
	t_AST_node	*top_node_ast;
	t_leaf	*final;

	if (!hashmap_init_basics(&data.environment, envvar_free))
		return (EXIT_FAILURE);
	if (!env_populate(envp, &data.environment))
	{
		hashmap_free_content(&data.environment);
		return (EXIT_FAILURE);
	}
	tokens = NULL;
	if (create_tokens(&tokens, input, &data) == EXIT_FAILURE
		|| check_syntax_errors(tokens, &data) == EXIT_FAILURE)
		exit(2);
	print_debug_lst(tokens, LEXEME_AND_TYPE, 2,
		 "\ndisplay tokens just after creating it\n");
	command_tab = create_cmd_tab(tokens, &data);
	print_debug_all_cmd(command_tab, LEXEME_AND_TYPE, 3,
		 "\ndisplay command_tab just after creating it\n");
	handle_all_here_doc(command_tab, &data);
	print_debug_all_cmd(command_tab, LEXEME_AND_TYPE, 5
		,"\ndisplay command_tab after handle here doc\n");
	top_node_ast = create_ast(command_tab, &data);
	check_malloc(top_node_ast, &data);
	final = evaluate_ast(top_node_ast, &data);
	execute_cmd(final, &data);
	rm_here_doc_files(command_tab);
	//cc -I includes/ -I libft/ src/utils/strjoin_alt.c src/data_structures/*.c src/lexing/*.c src/parsing/*.c src/test_parsing.c libft/libft.a -lreadline
}
