/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_substitution_in_lexeme.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:08:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/22 21:48:02 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_cmd_sub_in_lexeme(char *str, t_minishell *data);
size_t	in_parenthesis_len(char *str);
void	trim_nl_in_end(char *str);

static void	secure_cmd_sub_dup2(int pipefd[2], t_minishell *data)
{
	close(pipefd[0]);
	if (dup2(pipefd[1], 1) == -1)
	{
		close(pipefd[1]);
		raise_error_category("dup2", data);
	}
	close(pipefd[1]);
}

static char	*stock_file_in_str(int fd, t_minishell *data)
{
	char			*str;
	t_gnl_result	gnl_result;
	char			*buff;

	str = ft_calloc(1, sizeof(char));
	gnl_result = get_next_line(fd);
	if (gnl_result.error == 1)
		raise_error(data);
	buff = gnl_result.line;
	while (buff)
	{
		str = ft_strjoin_alt(str, buff, FREE_PARAM1 | FREE_PARAM2);
		if (str == NULL)
			raise_error(data);
		gnl_result = get_next_line(fd);
		if (gnl_result.error == 1)
			raise_error(data);
		buff = gnl_result.line;
	}
	check_malloc(str, data);
	return (str);
}

static char	*cmd_sub_result(char *str, size_t in_par_len, t_minishell *data)
{
	int			pipefd[2];
	pid_t		pid;

	str = ft_substr(str, 0, in_par_len);
	check_malloc(str, data);
	if (str[0] == '\0')
		return (str);
	if (pipe(pipefd) == -1)
		raise_error_category("pipe", data);
	pid = s_fork(data);
	if (pid == 0)
	{
		secure_cmd_sub_dup2(pipefd, data);
		parsing_exec(str, data);
		exit_minishell(data);
	}
	else if (pid == -1)
		raise_error_category("fork", data);
	close(pipefd[1]);
	waitpid(pid, NULL, 0);
	str = stock_file_in_str(pipefd[0], data);
	trim_nl_in_end(str);
	close(pipefd[0]);
	return (str);
}

static char	*cmd_substitution(char *str, char *cmd_sub_str, t_minishell *data)
{
	size_t	in_par_len;
	char	*until_cmd_sub;
	char	*after_cmd_sub;
	char	*final_result;

	in_par_len = in_parenthesis_len(cmd_sub_str);
	until_cmd_sub = ft_strjoin_alt_gc(str, cmd_sub_result(cmd_sub_str,
				in_par_len, data), FREE_PARAM2, data);
	check_malloc(until_cmd_sub, data);
	after_cmd_sub = handle_cmd_sub_in_lexeme(cmd_sub_str + in_par_len + 1,
			data);
	final_result = ft_strjoin_alt_gc(until_cmd_sub, until_cmd_sub,
			FREE_PARAM1 | FREE_PARAM2, data);
	check_malloc(final_result, data);
	return (final_result);
}

char	*handle_cmd_sub_in_lexeme(char *str, t_minishell *data)
{
	size_t	i;
	char	*str_dup;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '(')
		{
			str[i] = '\0';
			i += 2;
			return (cmd_substitution(str, str + i, data));
		}
		i++;
	}
	str_dup = ft_strdup(str);
	check_malloc(str_dup, data);
	return (str_dup);
}
