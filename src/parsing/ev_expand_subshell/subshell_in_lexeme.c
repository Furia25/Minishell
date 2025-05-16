/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_in_lexeme.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:08:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/16 22:21:47 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*stock_file_in_str(int fd, t_minishell *data)
{
	char	*str;
	t_gnl_result gnl_result;
	char	*buff;

	str = ft_calloc(1, sizeof(char));
	gnl_result = get_next_line(fd);
	if (gnl_result.error == 1 && gnl_result.line == NULL)
		malloc_error(data);
	buff = gnl_result.line;
	while (buff)
	{
		str = ft_strjoin_alt(str, buff, FREE_PARAM1 | FREE_PARAM2);
		if (str == NULL)
			malloc_error(data);
		gnl_result = get_next_line(fd);
		if (gnl_result.error == 1 && gnl_result.line == NULL)
			malloc_error(data);
		buff = gnl_result.line;
	}
	check_malloc(str, data);
	return (str);
}

static size_t	in_parenthesis_len(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != ')')
		i++;
	return (i);
}

static char	*subshell_str(char *str, size_t in_par_len, t_minishell *data)
{
	int		pipefd[2];
	pid_t		pid;

	str = ft_substr(str, 0, in_par_len);
	check_malloc(str, data);
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		parsing_exec(str, data);
		exit(0);
	}
	close(pipefd[1]);
	waitpid(pid, NULL, 0);
	str = stock_file_in_str(pipefd[0], data);
	close(pipefd[0]);
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
			buff = ft_substr(str, 0, i);
			check_malloc(buff, data);
			i += 2;
			in_par_len = in_parenthesis_len(str + i);
			buff = ft_strjoin_alt_gc(buff, subshell_str(str + i,
				in_par_len, data), FREE_PARAM1 | FREE_PARAM2, data);
			check_malloc(buff, data);
			return (check_malloc(ft_strjoin_alt_gc(buff,
				handle_subshell_in_lexeme(str + i + in_par_len + 1, data),
				FREE_PARAM1 | FREE_PARAM2, data), data));
		}
		i++;
	}
	return (check_malloc(ft_substr(str, 0, ft_strlen(str)), data));
}
