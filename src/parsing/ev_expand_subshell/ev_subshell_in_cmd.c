/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_subshell_in_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:06:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/01 23:24:24 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*handle_ev_in_lexeme(char *str, t_lexeme_type next_type, t_minishell *data);
char	*handle_subshell_in_lexeme(char *str, t_minishell *data);

static void	add_dollars_changes_in_lexeme(t_lst *token, t_minishell *data)
{
	char	*old_lexeme;

	if (token->type == DOUBLE_Q || token->next == NULL)
		old_lexeme = handle_ev_in_lexeme(token->lexeme, LINE_CHANGE, data);
	else
		old_lexeme = handle_ev_in_lexeme(token->lexeme, token->next->type, data);
	gc_free(token->lexeme, data);
	token->lexeme = handle_subshell_in_lexeme(old_lexeme, data);
	gc_free(old_lexeme, data);
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

static t_lst	*create_dollars_lst(t_lst *token, t_minishell *data)
{
	t_lst	 *dollars_lst;
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

static void	create_and_add_dollars_nodes(t_lst *token, t_leaf *command_tab, t_minishell *data)
{
	t_lst	*dollars_lst;
	char	*old_lexeme;

	old_lexeme = token->lexeme;
	if (ft_strchr("\n\t ", old_lexeme[0]) != NULL)
		token->type = DOLLAR;
	if (old_lexeme[0] == '\0')
		return ;
	if (ft_strchr("\n\t ",
		old_lexeme[ft_strlen(old_lexeme) - 1]) != NULL)
		token->metacharacter_after = true;
	token->lexeme = ft_strtrim(old_lexeme, "\n\t ");
	check_malloc(token->lexeme, data);
	gc_free(old_lexeme, data);
	dollars_lst = create_dollars_lst(token, data);
	if (dollars_lst != NULL)
	{
		command_tab->tokens = dollars_lst;
		gc_free_node(token, data);
		lstlast(dollars_lst)->next = token->next;
	}
}

void	ev_subshell_in_cmd(t_leaf *command_tab, t_minishell *data)
{
	t_lst	*temp;

	temp = command_tab->tokens;
	while (temp)
	{
		if (temp->type == WORD || temp->type == DOUBLE_Q)
			add_dollars_changes_in_lexeme(temp, data);
		temp = temp->next;
	}
	temp = command_tab->tokens;
	if (temp->type == WORD)
		create_and_add_dollars_nodes(temp, command_tab, data);
	while (temp->next)
	{
		if (temp->next->type == WORD)
			create_and_add_dollars_nodes(temp->next, command_tab, data);
		temp = temp->next;
	}
}
