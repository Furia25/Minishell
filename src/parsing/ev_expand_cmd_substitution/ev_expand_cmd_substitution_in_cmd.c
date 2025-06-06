/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_cmd_substitution_in_cmd.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:11:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/31 20:59:57 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_all_ev_expand(char *str, t_lexeme_type next_type,
			t_minishell *data);
char	*handle_all_cmd_substitution(char *str, t_minishell *data);
t_lst	*del_node_empty_lexeme(t_lst *prev, t_lst *current, t_leaf *cmd,
			t_minishell *data);
void	check_blank_in_extremity(t_lst *token, t_minishell *data);

void	add_dollars_changes_in_lexeme(t_lst *token, t_minishell *data)
{
	char	*old_lexeme;

	old_lexeme = handle_all_cmd_substitution(token->lexeme, data);
	gc_free(token->lexeme, data);
	if (token->type == DOUBLE_Q || token->next == NULL)
		token->lexeme = handle_all_ev_expand(old_lexeme,
				LINE_CHANGE, data);
	else
		token->lexeme = handle_all_ev_expand(old_lexeme,
				token->next->type, data);
	gc_free(old_lexeme, data);
}

static t_lst	*create_set_new_node(t_lst *token, size_t start, size_t len,
		t_minishell *data)
{
	char	*node_lexeme;
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
	t_lst	*dollars_lst;
	size_t	i;
	size_t	j;

	dollars_lst = NULL;
	i = 0;
	while ((token->lexeme)[i] != '\0')
	{
		while (ft_strchr(BASIC_WHITESPACE, (token->lexeme)[i]) != NULL)
			i++;
		j = 0;
		while (ft_strchr(BASIC_WHITESPACE, (token->lexeme)[i + j]) == NULL)
			j++;
		if (j == ft_strlen(token->lexeme))
			return (NULL);
		lstadd_back(&dollars_lst, create_set_new_node(token, i, j, data));
		i = i + j;
	}
	return (dollars_lst);
}

t_lst	*create_and_add_dollars_nodes(t_lst *prev, t_lst *current,
		t_leaf *cmd, t_minishell *data)
{
	t_lst	*dollars_lst;
	t_lst	*last_dollars_node;

	if (*(current->lexeme) == '\0')
		return (del_node_empty_lexeme(prev, current, cmd, data));
	check_blank_in_extremity(current, data);
	dollars_lst = create_dollars_lst(current, data);
	if (dollars_lst != NULL)
	{
		if (prev == NULL)
			cmd->tokens = dollars_lst;
		else
			prev->next = dollars_lst;
		last_dollars_node = lstlast(dollars_lst);
		last_dollars_node->next = current->next;
		gc_free_node(current, data);
		return (last_dollars_node);
	}
	return (current);
}

void	ev_expand_cmd_substitution_in_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst	*temp;

	temp = cmd->tokens;
	if (temp->type == WORD || temp->type == DOUBLE_Q)
		add_dollars_changes_in_lexeme(temp, data);
	if (temp->type == WORD)
		temp = create_and_add_dollars_nodes(NULL, temp, cmd, data);
	while (temp != NULL && temp->next)
	{
		if (temp->next->type == WORD || temp->next->type == DOUBLE_Q)
			add_dollars_changes_in_lexeme(temp->next, data);
		if (temp->next->type == WORD)
			temp = create_and_add_dollars_nodes(temp, temp->next, cmd, data);
		else
			temp = temp->next;
	}
}
