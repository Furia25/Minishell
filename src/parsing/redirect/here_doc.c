/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:43:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/14 22:32:52 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int		open_new_here_doc_file(t_leaf *command_tab, char **here_doc_file, t_minishell *data);
void	write_in_here_doc_file(t_lst *token_eof, int fd, t_minishell *data);

static char	*handle_here_doc(t_leaf *command_tab, t_lst	*token_eof, t_minishell *data)
{
	char	*here_doc_file;
	int	fd;

	if (command_tab->fd_input == -1)
		return (NULL);
	here_doc_file = ft_strdup("/tmp/here_doc");
	check_malloc(here_doc_file, data);
	fd = open_new_here_doc_file(command_tab, &here_doc_file, data);
	write_in_here_doc_file(token_eof, fd, data);
	close(fd);
	return (here_doc_file);
}

static void	here_doc_to_red_input(t_leaf *command_tab, t_minishell *data)
{
	t_lst	*temp;
	char	*here_doc_file;

	temp = command_tab->tokens;
	while (temp)
	{
		if (temp->type == HERE_DOC)
		{
			here_doc_file = handle_here_doc(command_tab, temp->next, data);
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
	while (command_tab->ope_after != LINE_CHANGE)
	{
		here_doc_to_red_input(command_tab, data);
		command_tab++;
	}
	here_doc_to_red_input(command_tab, data);
}

void	rm_all_here_doc_files(t_leaf *command_tab)
{
	t_lst	*tokens;
	
	while (command_tab->ope_after != LINE_CHANGE)
	{
		tokens = command_tab->tokens;
		while (tokens)
		{
			if (tokens->type == RED_IN &&
				ft_strncmp("/tmp/here_doc", tokens->next->lexeme, 13) == 0)
				unlink(tokens->next->lexeme);
			tokens = tokens->next;
		}
		command_tab++;
	}
	tokens = command_tab->tokens;
	while (tokens)
	{
		if (tokens->type == RED_IN &&
			ft_strncmp("/tmp/here_doc", tokens->next->lexeme, 13) == 0)
			unlink(tokens->next->lexeme);
		tokens = tokens->next;
	}
	
}



