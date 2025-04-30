/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:37:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/30 15:49:25 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
char	*handle_ev_in_here_doc(char *str, t_minishell *data);
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
			gc_free(*here_doc_file, data);
			*here_doc_file = ft_strjoin_alt_buff("/tmp/here_doc", ft_itoa(i), FREE_PARAM2, data);
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

bool	unclosed_par_here_doc(char *str, t_minishell *data)
{
	size_t	i;
	size_t	index_last_closed_par;

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
			{
				data->exit_code = 1;
				return (true);
			}
		}
		i++;
	}
	return (false);
}

char	*handle_dollars_in_here_doc(bool unclosed_par, char *input,
	t_lst *token_eof, t_minishell *data)
{
	char	*buff;

	if (token_eof->type != SINGLE_Q)
	{
		if (unclosed_par == false)
		{
			buff = handle_ev_in_here_doc(input, data);
			gc_free(input, data);
			input = handle_subshell_in_lexeme(buff, data);
			gc_free(buff, data);
		}
	}
	if (DEBUG == 4 || DEBUG == 1)
		ft_printf("input: %s\n", input);
	return (input);
}

void	write_in_here_doc_file(t_lst *token_eof, int fd, t_minishell *data)
{
	char	*input;
	char	*buff;
	bool	unclosed_par;

	unclosed_par = false;
	while (1)
	{
		input = readline("> ");
		if (unclosed_par_here_doc(input, data) == true)
			unclosed_par = true;
		check_malloc(input, data);
		if (ft_strcmp(input, token_eof->lexeme) == 0)
		{
			if (unclosed_par == true)
			{
				ft_putstr_fd("minishell: unexpected EOF ", 2);
				ft_putendl_fd("while looking for matching `)'", 2);
			}
			gc_free(input, data);
			break ;
		}
		handle_dollars_in_here_doc(unclosed_par, input, token_eof, data);
		ft_putendl_fd(input, fd);
		gc_free(input, data);
	}
}



