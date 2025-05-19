/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:43:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/19 03:38:10 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"

int		open_new_here_doc_file(char **here_doc_file, t_minishell *data);
int		write_in_here_doc_file(int fd, t_lst *token_eof, t_minishell *data);

static char	*handle_here_doc(t_leaf *cmd, t_lst *token_eof, t_minishell *data)
{
	char	*here_doc_file;
	int		here_doc_error;

	if (cmd->fd_input == -1)
		return (NULL);
	here_doc_file = ft_strdup("/tmp/here_doc");
	check_malloc(here_doc_file, data);
	cmd->fd_input = open_new_here_doc_file(&here_doc_file, data);
	if (cmd->fd_input == -1)
		return (here_doc_file);
	here_doc_error = write_in_here_doc_file(cmd->fd_input, token_eof, data);
	close(cmd->fd_input);
	if (here_doc_error != 0)
		cmd->fd_input = -1;
	if (here_doc_error == 0)
		cmd->fd_input = 0;
	if (here_doc_error == 2)
		return (NULL);
	return (here_doc_file);
}

static void	here_docs_in_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst	*temp;
	char	*here_doc_file;

	temp = cmd->tokens;
	while (temp)
	{
		if (temp->type == HERE_DOC)
		{
			here_doc_file = handle_here_doc(cmd, temp->next, data);
			gc_free(temp->next->lexeme, data);
			temp->type = RED_IN;
			temp->next->lexeme = here_doc_file;
			temp = temp->next->next;
		}
		else
			temp = temp->next;
	}
}

void	handle_all_here_doc(t_leaf *command_tab, t_minishell *data)
{
	setup_signals(SIGCONTEXT_HEREDOC);
	while (command_tab->ope_after != LINE_CHANGE)
	{
		g_signal_status = 0;
		here_docs_in_cmd(command_tab, data);
		command_tab++;
	}
	g_signal_status = 0;
	here_docs_in_cmd(command_tab, data);
	setup_signals(SIGCONTEXT_PARENT);
}

void	rm_here_doc_files_in_cmd(t_lst *tokens)
{
	while (tokens)
	{
		if (tokens->type == RED_IN &&
			ft_strncmp("/tmp/here_doc", tokens->next->lexeme, 13) == 0)
			unlink(tokens->next->lexeme);
		tokens = tokens->next;
	}
}

void	rm_all_here_doc_files(t_leaf *command_tab)
{
	t_lst	*tokens;
	
	while (command_tab->ope_after != LINE_CHANGE)
	{
		tokens = command_tab->tokens;
		rm_here_doc_files_in_cmd(tokens);
		command_tab++;
	}
	tokens = command_tab->tokens;
	rm_here_doc_files_in_cmd(tokens);
}



