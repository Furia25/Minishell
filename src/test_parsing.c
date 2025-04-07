/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:50:30 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/31 16:21:35 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char *input = "		abcde	  fezze |	agh	||	'ffezz'	";
	t_list	*tokens;
	t_leaf *command_tab;
	t_leaf	*final;

	tokens = NULL;
	create_tokens(&tokens, input);
	command_tab = create_cmd_tab(tokens);
	final = evaluate_ast(create_ast(command_tab));
	printf("stdin: %c\nstdout: %c\nvalue: %d\n\n", final->cmd_stdin, final->cmd_stdout, final->returned_value);
	
}
