/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 23:37:34 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/17 13:42:56 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char *input = "<<eof cat | cat > /root/ && cat > caca";
	t_lst	*tokens;
	t_leaf *command_tab;
	t_leaf	*final;

	tokens = NULL;
	create_tokens(&tokens, input);
	check_syntax_errors(tokens);
	fusion_quote_token(tokens);
	command_tab = create_cmd_tab(tokens);
	handle_all_here_doc(command_tab);
	final = evaluate_ast(create_ast(command_tab));
	execute_cmd(final);
	rm_here_doc_files(command_tab);
	//printf("token: %s\nstdin: %d\nstdout: %d\nvalue: %d\n\n", (char*)final->tokens->content, final->fd_input, final->fd_output, final->returned_value);
	//cc -I includes/ -I libft/ src/utils/strjoin_alt.c src/data_structures/*.c src/lexing/*.c src/parsing/*.c src/test_parsing.c libft/libft.a -lreadline
}
