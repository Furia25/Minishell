/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:37:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 04:43:28 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
char	*handle_subshell_in_lexeme(char *str, t_minishell *data);

int	open_new_here_doc_file(t_leaf *command_tab, char **here_doc_file, t_minishell *data)
{
	int		fd;
	int		i;

	fd = open(*here_doc_file, O_WRONLY | O_CREAT | O_EXCL, 0644);
	if (fd == -1 && errno == EEXIST)
	{
		i = 1;
		while (errno == EEXIST)
		{
			free(*here_doc_file);
			*here_doc_file = ft_strjoin_alt("/tmp/here_doc", ft_itoa(i), FREE_PARAM2);
			check_malloc(*here_doc_file, data);
			fd = open(*here_doc_file, O_WRONLY | O_CREAT | O_EXCL, 0644);
			if (fd != -1)
				break;
			i++;
		}
	}
	if (fd == -1 && errno != EEXIST)
	{
		command_tab->fd_input = -1;
		perror("here doc");
		exit(1);//a secur
	}
	return (fd);
}

void	write_in_here_doc_file(t_lst *token_eof, int fd, t_minishell *data)
{
	char	*input;
	char	*buff;

	while (1)
	{
		input = readline("> ");
		check_malloc(input, data);
		if (ft_strcmp(input, token_eof->lexeme) == 0)
		{
			free(input);
			break ;
		}
		if (token_eof->type != SINGLE_Q)
		{
			buff = handle_subshell_in_lexeme(input, data);
			free(input);
			input = buff;
		}
		if (DEBUG == 4 || DEBUG == 1)
			ft_printf("input: %s\n", input);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
}
