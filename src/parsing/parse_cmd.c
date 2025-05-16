/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 03:43:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/16 03:39:32 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_cmd(t_leaf *cmd, t_minishell *data)
{
	int	fd_here_doc;

	fd_here_doc = cmd->fd_input;

	if (fd_here_doc == -1)
		return ;
	ev_subshell_in_cmd(cmd, data);
	print_debug_lst(cmd->tokens, LEXEME | TYPE, 7,
		"\ndisplay command->tokens after handle ev_expension and subshell\n");
	fusion_quote_token(cmd->tokens, data);
	print_debug_lst(cmd->tokens, LEXEME, 8,
		"\ndisplay command->tokens after handle fusion quotes\n");
	wildcards_in_cmd(cmd, data);
	print_debug_lst(cmd->tokens, LEXEME | TYPE, 9,
		"\ndisplay command->tokens after handle wildcards\n");
	redirections_in_cmd(cmd, data);
	print_debug_cmd(cmd, LEXEME, 10,
		"\ndisplay command after handle redi\n");
}
