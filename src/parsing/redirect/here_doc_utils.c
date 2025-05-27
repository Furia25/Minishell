/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:37:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/27 17:50:50 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include "redirections.h"
#include "debug.h"

char	*handle_all_ev_expand_in_here_doc(char *str, t_minishell *data);
char	*handle_all_cmd_substitution(char *str, t_minishell *data);

int	open_new_here_doc_file(char **here_doc_file, t_minishell *data)
{
	int		fd;
	int		i;

	fd = open(*here_doc_file, O_WRONLY | O_CREAT | O_EXCL, 0644);
	if (fd == -1 && errno == EEXIST)
	{
		i = 1;
		while (errno == EEXIST)
		{
			gc_free(*here_doc_file, data);
			*here_doc_file = ft_strjoin_alt(HERE_DOC_FILE,
					ft_itoa(i), FREE_PARAM2);
			check_malloc(*here_doc_file, data);
			fd = open(*here_doc_file, O_WRONLY | O_CREAT | O_EXCL, 0644);
			if (fd != -1)
				break ;
			i++;
		}
	}
	if (fd == -1 && errno != EEXIST)
	{
		print_basic_error("here-doc");
		return (-1);
	}
	return (fd);
}

bool	unclosed_par_here_doc(char *str)
{
	size_t	i;
	size_t	index_last_closed_par;

	if (!str)
		return (false);
	i = 0;
	while (str[i])
	{
		index_last_closed_par = 0;
		if (str[i] == '$' && str[i + 1] == '(')
		{
			while (str[i] != '\0')
			{
				if (str[i] == ')')
					index_last_closed_par = i;
				i++;
			}
			i = index_last_closed_par;
			if (index_last_closed_par == 0)
				return (true);
		}
		i++;
	}
	return (false);
}

char	*handle_dollars_in_here_doc(bool unclosed_par, char **input,
	t_lexeme_type type_eof, t_minishell *data)
{
	char	*buff;

	if (type_eof != SINGLE_Q)
	{
		if (unclosed_par == false)
		{
			buff = handle_all_ev_expand_in_here_doc(*input, data);
			gc_free(*input, data);
			*input = handle_all_cmd_substitution(buff, data);
			gc_free(buff, data);
		}
	}
	if (DEBUG == 4 || DEBUG == 1)
		ft_printf_fd(STDERR_FILENO, "input: %s\n", *input);
	return (*input);
}

ssize_t	secure_putendl_fd(char *s, int fd, t_minishell *data)
{
	ssize_t	s_write;
	ssize_t	nl_write;

	s_write = write(fd, s, ft_strlen(s));
	nl_write = write(fd, "\n", 1);
	gc_free(s, data);
	if (s_write == -1 || nl_write == -1)
	{
		print_basic_error("here-doc");
		return (-1);
	}
	return (s_write + nl_write);
}
