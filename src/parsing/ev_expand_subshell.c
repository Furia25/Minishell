/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_subshell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:47:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 01:51:45 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*handle_ev_in_lexeme(char *str, t_lexeme_type next_type, t_minishell *data);

char	*stock_file_in_str(int fd, t_minishell *data)
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

size_t	in_parenthesis_len(char *str, t_minishell *data)
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

size_t	env_var_len(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '$')
		i++;
	return (i);
}

char	*ev_result(char *str, size_t ev_len, t_minishell *data)
{
	return (ft_substr("  d  ", 0, 5));
}

size_t	skip_subshell(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '$' && str[i + 1] == '(')
	{
		while (str[i] != ')')
			i++;
	}
	return (i);
}

char	*ev_expand(char *str, char *ev_str, t_lexeme_type next_type, t_minishell *data)
{
	size_t	ev_len;
	char	*buff;

	ev_len = env_var_len(ev_str);
	if (ev_len == 0)
		return (ft_calloc(1, sizeof(char)));
	buff = ft_strjoin_alt(str, ev_result(ev_str, ev_len, data), FREE_PARAM2);
	check_malloc(buff, data);
	return (check_malloc(ft_strjoin_alt(buff,
		handle_ev_in_lexeme(ev_str + ev_len, next_type, data),
		FREE_PARAM1 | FREE_PARAM2), data));
}

char	*handle_ev_in_lexeme(char *str, t_lexeme_type next_type, t_minishell *data)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		skip_subshell(str + i);
		if (str[i] == '$' && str[i + 1] != '(')
		{
			if ((str[i + 1] != '\0'
				|| (next_type == DOUBLE_Q || next_type == SINGLE_Q)))
			{
				str[i] = '\0';
				i++;
				return (ev_expand(str, str + i, next_type, data));
			}
		}
		i++;
	}
	return (check_malloc(ft_substr(str, 0, ft_strlen(str)), data));
}

char	*subshell_str(char *str, size_t in_par_len, t_minishell *data)
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
			buff = ft_strjoin_alt(str, subshell_str(str + i,
				in_par_len, data), FREE_PARAM2);
			check_malloc(buff, data);
			return (check_malloc(ft_strjoin_alt(buff,
				handle_subshell_in_lexeme(str + i + in_par_len + 2, data),
				FREE_PARAM1 | FREE_PARAM2), data));
		}
		i++;
	}
	return (check_malloc(ft_substr(str, 0, ft_strlen(str)), data));
	// juste return str si free a enlever dans strjoin
}

static t_lst	*create_set_new_node(t_lst *token, size_t start, size_t len, t_minishell *data)
{
	char *node_lexeme;
	t_lst	*new_node;

	node_lexeme = ft_substr(token->lexeme, start, len);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	if (start == 0)
		new_node->type = token->type;
	else
		new_node->type = DOLLAR;
	new_node->metacharacter_after = token->metacharacter_after;
	return (new_node);
}

t_lst	*create_dollars_lst(t_lst *token, t_minishell *data)
{
	t_lst	*dollars_lst;
	size_t	i;
	size_t	j;

	dollars_lst = NULL;
	i = 0;
	while ((token->lexeme)[i] != '\0')
	{
		j = 0;
		while (ft_strchr("\n\t ", (token->lexeme)[i + j]) == NULL)
			j++;
		if (j == ft_strlen(token->lexeme))
			return (NULL);
		lstadd_back(&dollars_lst, create_set_new_node(token, i, j, data));
		while (ft_strchr("\n\t ", (token->lexeme)[i + j]) != NULL)
		{
			if ((token->lexeme)[i + j] == '\0')
				return (dollars_lst);
			j++;
		}
		i = i + j;
	}
	return (dollars_lst);
}

void	add_dollars_changes_in_lexeme(t_lst *token, t_minishell *data)
{
	char	*old_lexeme;

	old_lexeme = token->lexeme;
	if (token->type == DOUBLE_Q || token->next == NULL)
		old_lexeme = handle_ev_in_lexeme(old_lexeme, LINE_CHANGE, data);
	else
		old_lexeme = handle_ev_in_lexeme(old_lexeme, token->next->type, data);
	token->lexeme = handle_subshell_in_lexeme(old_lexeme, data);
	free(old_lexeme);
}

void	create_add_dollars_nodes(t_lst *token, t_leaf *command_tab, t_minishell *data)
{
	t_lst	*dollars_lst;
	char	*old_lexeme;

	old_lexeme = token->lexeme;
	if (ft_strchr("\n\t ", old_lexeme[0]) != NULL)
		token->type = DOLLAR;
	if (ft_strchr("\n\t ",
		old_lexeme[ft_strlen(old_lexeme) - 1]) != NULL)
		token->metacharacter_after = true;
	token->lexeme = ft_strtrim(old_lexeme, "\n\t ");
	check_malloc(token->lexeme, data);
	free(old_lexeme);
	dollars_lst = create_dollars_lst(token, data);
	if (dollars_lst != NULL)
	{
		command_tab->tokens = dollars_lst;
		lstdelone(token, free);
		lstlast(dollars_lst)->next = token->next;
	}
}

void	ev_subshell_in_cmd(t_leaf *command_tab, t_minishell *data)
{
	t_lst	*temp;
	t_lst	*dollars_lst;
	char	*old_lexeme;

	temp = command_tab->tokens;
	while (temp)
	{
		if (temp->type == WORD || temp->type == DOUBLE_Q)
		{
			add_dollars_changes_in_lexeme(temp, data);
			if (*(temp->lexeme) == '\0')
				return ;
		}
		temp = temp->next;
	}
	temp = command_tab->tokens;
	if (temp->type == WORD)
		create_add_dollars_nodes(temp, command_tab, data);
	while (temp->next)
	{
		if (temp->next->type == WORD)
			create_add_dollars_nodes(temp->next, command_tab, data);
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
