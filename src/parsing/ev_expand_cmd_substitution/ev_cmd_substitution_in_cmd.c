/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_cmd_substitution_in_cmd.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:26:14 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/22 18:26:16 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_ev_in_lexeme(char *str, t_lexeme_type next_type,
			t_minishell *data);
char	*handle_cmd_sub_in_lexeme(char *str, t_minishell *data);
t_lst	*ignore_void_lexeme(t_lst *prev, t_lst *current, t_leaf *cmd,
			t_minishell *data);
void	check_blank_in_extremity(t_lst *token, t_minishell *data);

static void	add_dollars_changes_in_lexeme(t_lst *token, t_minishell *data)
{
	char	*old_lexeme;

	if (token->type == DOUBLE_Q || token->next == NULL)
		old_lexeme = handle_ev_in_lexeme(token->lexeme, LINE_CHANGE, data);
	else
		old_lexeme = handle_ev_in_lexeme(token->lexeme, token->next->type,
				data);
	gc_free(token->lexeme, data);
	token->lexeme = handle_cmd_sub_in_lexeme(old_lexeme, data);
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
		while (ft_strchr("\n\t ", (token->lexeme)[i]) != NULL)
			i++;
		j = 0;
		while (ft_strchr("\n\t ", (token->lexeme)[i + j]) == NULL)
			j++;
		if (j == ft_strlen(token->lexeme))
			return (NULL);
		lstadd_back(&dollars_lst, create_set_new_node(token, i, j, data));
		i = i + j;
	}
	return (dollars_lst);
}

static t_lst	*create_and_add_dollars_nodes(t_lst *prev, t_lst *current,
		t_leaf *cmd, t_minishell *data)
{
	t_lst	*dollars_lst;
	t_lst	*last_dollars_node;

	if (*(current->lexeme) == '\0')
		return (ignore_void_lexeme(prev, current, cmd, data));
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

void	ev_cmd_substitution_in_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst	*temp;

	temp = cmd->tokens;
	while (temp)
	{
		if (temp->type == WORD || temp->type == DOUBLE_Q)
			add_dollars_changes_in_lexeme(temp, data);
		temp = temp->next;
	}
	temp = cmd->tokens;
	if (temp->type == WORD)
		temp = create_and_add_dollars_nodes(NULL, cmd->tokens, cmd, data);
	while (temp != NULL && temp->next)
	{
		if (temp->next->type == WORD)
			temp = create_and_add_dollars_nodes(temp, temp->next, cmd, data);
		else
			temp = temp->next;
	}
}
