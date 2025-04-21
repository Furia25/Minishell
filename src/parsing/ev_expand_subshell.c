/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_subshell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:47:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/21 02:42:00 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	not_interpret_chara(char chara, char *str);

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

char	*handle_subshell_in_lexeme(char *str, size_t *i_ptr)
{
	size_t	j;
	int		pipefd[2];
	int		pipefd2[2];
	pid_t		pid;

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
	write(pipefd[1], "test1   test2", 13);
	close(pipefd[1]);
	wait(NULL);
	str = stock_file_in_str(pipefd2[0]);
	close(pipefd2[0]);
	return (str);
}

char	*handle_dollars_in_lexeme(char *str)
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
				buff = ft_strjoin_alt(str, handle_subshell_in_lexeme(str + i, &i), FREE_PARAM2);
				return (ft_strjoin_alt(buff, handle_dollars_in_lexeme(str + i), FREE_PARAM1 | FREE_PARAM2));
			}
		}
		i++;
	}
	return (ft_substr(str, 0, ft_strlen(str)));
}

t_lst	*create_subshell_lst(t_lst *token)
{
	t_lst	*subshell_lst;
	size_t	i;
	size_t	j;
	char	*node_lexeme;
	t_lst	*new_node;

	subshell_lst = NULL;
	i = 0;
	while ((token->lexeme)[i] != '\0')
	{
		while (ft_strchr("\n\t ", (token->lexeme)[i]) != NULL)
			i++;
		j = 0;
		while (ft_strchr("\n\t ", (token->lexeme)[i + j]) == NULL)
			j++;
		if (j == ft_strlen(token->lexeme))
			return (NULL);
		node_lexeme = ft_substr(token->lexeme, i, j);
		new_node = lstnew(node_lexeme);
		new_node->type = SUBSHELL;
		new_node->metacharacter_after = token->metacharacter_after;
		lstadd_back(&subshell_lst, new_node);
		if ((token->lexeme)[i + j] != '\0')
			i = i + j + 1;
		else
			break ;
	}
	return (subshell_lst);
}

void	handle_subshell_in_cmd(t_leaf *command_tab)
{
	t_lst	*temp;
	t_lst	*subshell_lst;
	char	*new_lexeme;

	temp = command_tab->tokens;
	while (temp)
	{
		if (temp->type == WORD || temp->type == DOUBLE_Q)
		{
			new_lexeme = handle_dollars_in_lexeme(temp->lexeme);
			free(temp->lexeme);
			temp->lexeme = ft_strtrim(new_lexeme, "\n\t ");
			free(new_lexeme);
		}
		temp = temp->next;
	}
	temp = command_tab->tokens;
	if (temp->type == WORD)
	{
		subshell_lst = create_subshell_lst(temp);
		if (subshell_lst != NULL)
		{
			command_tab->tokens = subshell_lst;
			lstdelone(temp, free);
			lstlast(subshell_lst)->next = temp->next;
		}
	}
	while (temp->next)
	{
		if (temp->next->type == WORD)
		{
			subshell_lst = create_subshell_lst(temp->next);
			if (subshell_lst != NULL)
			{
				lstdelone(temp->next, free);
				lstlast(subshell_lst)->next = temp->next->next;
				temp->next = subshell_lst;
			}
		}
		temp = temp->next;
	}
}

// int	main(void)
// {
// 	char *input = "echo  df$(ls)a$(ls)dfd";
// 	t_lst	*tokens;
// 	t_leaf *command_tab;
// 	t_lst	*temp;

// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	check_syntax_errors(tokens);
// 	command_tab = create_cmd_tab(tokens);
// 	handle_subshell_in_cmd(command_tab);
// 	fusion_quote_token(command_tab->tokens);
// 	temp = command_tab->tokens;
// 	while (temp)
// 	{
// 		printf("\nNew node\n");
// 		printf("%s\n", temp->lexeme);
// 		temp = temp->next;
// 	}
// }