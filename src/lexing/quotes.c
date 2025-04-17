/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 23:49:57 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/18 01:56:11 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	not_interpret_chara(char chara, char *str);

size_t single_quote_token(t_lst **tokens, char *str)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 1;
	while (str[i] != '\0' && str[i] != '\'')
		i++;
	if (str[i] == '\0')
		not_interpret_chara('\'', "\' (unclosed single quote)");
	node_lexeme = ft_substr(str, 1, i - 1);//a secur
	new_node = lstnew(node_lexeme);//a secur
	new_node->type = SINGLE_Q;
	if (ft_strchr("|&;()<> ", str[i + 1]) != NULL)
		new_node->metacharacter_after = true;
	else
		new_node->metacharacter_after = false;
	lstadd_back(tokens, new_node);
	return (i + 1);
}

size_t double_quote_token(t_lst **tokens, char *str)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 1;
	while (str[i] != '\0' && str[i] != '\"')
		i++;
	if (str[i] == '\0')
		not_interpret_chara('\"', "\' (unclosed double quote)");
	node_lexeme = ft_substr(str, 1, i - 1);//a secur
	new_node = lstnew(node_lexeme);//a secur
	new_node->type = DOUBLE_Q;
	if (ft_strchr("|&;()<> ", str[i + 1]) != NULL)
		new_node->metacharacter_after = true;
	else
		new_node->metacharacter_after = false;
	lstadd_back(tokens, new_node);
	return (i + 1);
}

char	*stock_file_in_str(int fd)
{
	char	*str;
	char	*buff;

	str = malloc(1);
	str[0] = '\0';
	buff = get_next_line(fd);
	while (buff)
	{
		str = ft_strjoin_alt(str, buff, FREE_PARAM1 | FREE_PARAM2);
		buff = get_next_line(fd);
	}
	return (str);
}

char	*subshell_in_DQ(char *str, size_t *i_ptr)
{
	size_t	j;
	int		pipefd[2];
	int		pipefd2[2];
	pid_t		pid;
	char	*result;

	j = 0;
	while (str[j] != '\0' && str[j] != ')')
		j++;
	if (str[j] == '\0')
		not_interpret_chara('(', "\' (unclosed parenthesis)");
	*i_ptr = *i_ptr + j + 1;
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
	write(pipefd[1], "str", 3);
	close(pipefd[1]);
	wait(NULL);
	result = stock_file_in_str(pipefd2[0]);
	close(pipefd2[0]);
	return (result);
}

char	*handle_dollars_in_DQ(char *str)
{
	size_t	i;
	char	*buff;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			str[i] = '\0';
			i++;
			if (str[i] == '(')
			{
				i++;
				buff = ft_strjoin_alt(str, subshell_in_DQ(str + i, &i), FREE_PARAM2);
				return (ft_strjoin_alt(buff, handle_dollars_in_DQ(str + i), FREE_PARAM1 | FREE_PARAM2));
			}
		}
		i++;
	}
	return (ft_substr(str, 0, ft_strlen(str)));
}

void	fusion_quote_token(t_lst *tokens)
{
	t_lst *temp;

	temp = tokens;
	while (temp)
	{
		if ((temp->type == SINGLE_Q || temp->type == DOUBLE_Q)
			&& temp->metacharacter_after == false)
		{
			temp->lexeme = ft_strjoin_alt(temp->lexeme, temp->next->lexeme,
				FREE_PARAM1);
			lstdelone(temp->next, free);
			temp->next = temp->next->next;
		}
		temp = temp->next;
	}
}

int	main(void)
{
	char	*str;

	str = "ab$(ls)c g$(ls)g    $(ls)";
	str = ft_substr(str, 0, ft_strlen(str));
	ft_printf("%s\n", handle_dollars_in_DQ(str));
}


