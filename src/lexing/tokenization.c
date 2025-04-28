/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:22:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 05:09:39 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
size_t single_quote_token(t_lst **tokens, char *str, t_minishell *data);
size_t double_quote_token(t_lst **tokens, char *str, t_minishell *data);
size_t word_token(t_lst **tokens, char *str, t_minishell *data);

static size_t op_control_token(t_lst **tokens, char *str, char op, t_minishell *data)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 0;
	while (str[i] == op && i != 2)
		i++;
	node_lexeme = ft_substr(str, 0, i);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	if (i == 1 && op == '|')
		new_node->type = PIPE;
	else if (i == 2 && op == '|')
		new_node->type = OR;
	else
		new_node->type = AND;
	lstadd_back(tokens, new_node);
	return (i);
}

static size_t op_redirection_token(t_lst **tokens, char *str, char op, t_minishell *data)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 0;
	while (str[i] == op && i != 2)
		i++;
	if (i == 1 && op == '<' && str[i] == '>')
		i++;
	node_lexeme = ft_substr(str, 0, i);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	if (i == 1 && op == '<')
		new_node->type = RED_IN;
	else if (i == 1 && op == '>')
		new_node->type = RED_OUT;
	else if (i == 2 && op == '<' && str[i - 1] != '>')
		new_node->type = HERE_DOC;
	else if (i == 2 && op == '<' && str[i - 1] == '>')
		new_node->type = RED_IN_OUT;
	else if (i == 2 && op == '>')
		new_node->type = RED_OUT_A;
	lstadd_back(tokens, new_node);
	return (i);
}

static size_t op_parenthesis(t_lst **tokens, char *str, char op, t_minishell *data)
{
	char *node_lexeme;
	t_lst	*new_node;

	node_lexeme = ft_substr(str, 0, 1);
	check_malloc(node_lexeme, data);
	new_node = lstnew(node_lexeme);
	check_malloc(new_node, data);
	if (op == '(')
		new_node->type = PAR_OPEN;
	if (op == ')')
		new_node->type = PAR_CLOSE;
	lstadd_back(tokens, new_node);
	return (1);
}

int	create_tokens(t_lst **tokens, char *input, t_minishell *data)
{
	size_t	token_len;

	token_len = 0;
	if (*input == '\0')
		return (EXIT_SUCCESS);
	while (*input == ' ' || *input == '\t')
		input++;
	if ((*input == '&' && *(input + 1) != '&') || *input == ';' || *input == '\\')
		return (not_interpret_chara(*input, "\'", data));
	if (ft_strchr("|&;()<> \t\'\"", *input) == NULL)
		token_len = word_token(tokens, input, data);
	if (*input == '\'')
		token_len = single_quote_token(tokens, input, data);
	if (*input == '\"')
		token_len = double_quote_token(tokens, input, data);
	if (*input == '(' || *input == ')')
		token_len = op_parenthesis(tokens, input, *input, data);
	if (*input == '|' || *input == '&')
		token_len = op_control_token(tokens, input, *input, data);
	if (*input == '<' || *input == '>')
		token_len = op_redirection_token(tokens, input, *input, data);
	if (token_len == 0)
		return (EXIT_FAILURE);
	return (create_tokens(tokens, input + token_len, data));
}
