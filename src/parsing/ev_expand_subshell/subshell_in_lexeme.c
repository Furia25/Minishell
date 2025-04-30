/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_in_lexeme.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:08:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/30 15:47:44 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*stock_file_in_str(int fd, t_minishell *data)
{
	char	*str;
	char	*buff;

	str = ft_calloc(1, sizeof(char));
	check_malloc(str, data);
	buff = get_next_line(fd);
	//check_malloc(buff, data);
	while (buff)
	{
		str = ft_strjoin_alt(str, buff, FREE_PARAM1 | FREE_PARAM2);
		check_malloc(str, data);
		buff = get_next_line(fd);
		//check_malloc(buff, data);
	}
	return (str);
}

static size_t	in_parenthesis_len(char *str, t_minishell *data)
{
	size_t	i;
	size_t	index_last_closed_par;

	i = 0;
	index_last_closed_par = 0;
	while (str[i])
	{
		if (str[i] == ')')
		index_last_closed_par = i;
		i++;
	}
	return (index_last_closed_par - 1);
}

static char	*subshell_str(char *str, size_t in_par_len, t_minishell *data)
{
	int		pipefd[2];
	int		pipefd2[2];
	pid_t		pid;

	(void)in_par_len;
	pipe(pipefd);
	pipe(pipefd2);
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[1]);
		close(pipefd2[0]);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		dup2(pipefd2[1], 1);
		close(pipefd2[1]);
		ft_printf("%s", get_next_line(0));
		exit(0);
	}
	close(pipefd[0]);
	close(pipefd2[1]);
	write(pipefd[1], "  ", 2);
	close(pipefd[1]);
	wait(NULL);
	str = stock_file_in_str(pipefd2[0], data);
	close(pipefd2[0]);
	return (str);
}

char	*handle_subshell_in_lexeme(char *str, t_minishell *data)
{
	size_t	i;
	size_t	in_par_len;
	char	*buff;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			str[i] = '\0';
			i++;
			in_par_len = in_parenthesis_len(str + i, data);
			buff = ft_strjoin_alt_gc(str, subshell_str(str + i,
				in_par_len, data), FREE_PARAM2, data);
			check_malloc(buff, data);
			return (check_malloc(ft_strjoin_alt_gc(buff,
				handle_subshell_in_lexeme(str + i + in_par_len + 2, data),
				FREE_PARAM1 | FREE_PARAM2, data), data));
		}
		i++;
	}
	return (check_malloc(ft_substr(str, 0, ft_strlen(str)), data));
	// juste return str si free a enlever dans strjoin
}