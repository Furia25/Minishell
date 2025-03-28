/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:50:30 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/28 04:37:26 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	main(void)
// {
// 	char *input = "<<eof <<ac cat | <<ac ls >prout >abc l";
// 	t_list	*tokens;
// 	t_leaf *command_tab;
// 	t_leaf	*final;

// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	command_tab = create_cmd_tab(tokens);
// 	handle_redirections(command_tab);
// 	final = evaluate_ast(create_ast(command_tab));
// 	execute_last_cmd(final);
// 	//printf("token: %s\nstdin: %d\nstdout: %d\nvalue: %d\n\n", (char*)final->tokens->content, final->fd_input, final->fd_output, final->returned_value);

// }
