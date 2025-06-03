/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 03:43:36 by alpayet           #+#    #+#             */
/*   Updated: 2025/06/03 19:56:28 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"
#include "minishell.h"
#include "wildcards.h"
#include "debug.h"

void	expand_tilde(t_leaf *cmd, t_minishell *data);
void	ev_expand_cmd_substitution_in_par_cmd(t_leaf *cmd, t_minishell *data);
void	merge_joined_tokens_in_par_cmd(t_lst *tokens, t_minishell *data);
void	wildcards_in_par_cmd(t_leaf *cmd, t_minishell *data);

void	parse_cmd(t_leaf *cmd, t_minishell *data)
{
	int	fd_here_doc;

	fd_here_doc = cmd->fd_input;
	if (fd_here_doc == -1)
		return ;
	expand_tilde(cmd, data);
	ev_expand_cmd_substitution_in_cmd(cmd, data);
	print_debug_lst(cmd->tokens, LEXEME | TYPE, 7,
		"\ndisplay command->tokens after handle ev_expand and cmd_sub\n");
	merge_joined_tokens_in_cmd(cmd->tokens, data);
	print_debug_lst(cmd->tokens, LEXEME, 8,
		"\ndisplay command->tokens after handle fusion quotes\n");
	wildcards_in_cmd(cmd, data);
	print_debug_lst(cmd->tokens, LEXEME | TYPE, 9,
		"\ndisplay command->tokens after handle wildcards\n");
	redirections_in_cmd(cmd, data);
	print_debug_cmd(cmd, LEXEME, 10,
		"\ndisplay command after handle redis\n");
}

void	parse_par_cmd(t_leaf *cmd, t_minishell *data)
{
	int	fd_here_doc;

	fd_here_doc = cmd->fd_input;
	if (fd_here_doc == -1)
		return ;
	ev_expand_cmd_substitution_in_par_cmd(cmd, data);
	print_debug_lst(cmd->tokens, LEXEME | TYPE, 7,
		"\ndisplay command->tokens after handle ev_expand and cmd_sub\n");
	merge_joined_tokens_in_par_cmd(cmd->tokens, data);
	print_debug_lst(cmd->tokens, LEXEME, 8,
		"\ndisplay command->tokens after handle fusion quotes\n");
	wildcards_in_par_cmd(cmd, data);
	print_debug_lst(cmd->tokens, LEXEME | TYPE, 9,
		"\ndisplay command->tokens after handle wildcards\n");
	redirections_in_par_cmd(cmd, data);
	print_debug_cmd(cmd, LEXEME, 10,
		"\ndisplay command after handle redis\n");
}
