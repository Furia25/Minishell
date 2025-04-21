/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 23:37:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/21 02:55:03 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

int	main(void)
{
	char *input = "cat <<$(ls)";
	t_lst	*tokens;
	t_leaf *command_tab;
	t_leaf	*final;

	tokens = NULL;
	create_tokens(&tokens, input);
	print_debug_lst(tokens, LEXEME_AND_TYPE, 2,
		 "\ndisplay tokens just after creating it\n");
	check_syntax_errors(tokens);
	command_tab = create_cmd_tab(tokens);
	print_debug_all_cmd(command_tab, LEXEME_AND_TYPE, 3,
		 "\ndisplay command_tab just after creating it\n");
	handle_all_here_doc(command_tab);
	print_debug_all_cmd(command_tab, LEXEME_AND_TYPE, 5
		,"\ndisplay command_tab after handle here doc\n");
	final = evaluate_ast(create_ast(command_tab));
	execute_cmd(final);
	rm_here_doc_files(command_tab);
	//cc -I includes/ -I libft/ src/utils/strjoin_alt.c src/data_structures/*.c src/lexing/*.c src/parsing/*.c src/test_parsing.c libft/libft.a -lreadline
}
