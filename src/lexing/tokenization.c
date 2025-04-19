/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:22:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/19 23:09:53 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
size_t single_quote_token(t_lst **tokens, char *str);
size_t double_quote_token(t_lst **tokens, char *str);
void	not_interpret_chara(char chara, char *str);

size_t word_token(t_lst **tokens, char *str)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 0;
	while (ft_strchr("|&;<>() \t\'\"", str[i]) == NULL)
	{
		if (str[i] == '$')
		{
			i++;
			while (str[i] != '\0' && str[i] != ')')
				i++;
			if (str[i] == '\0')
				not_interpret_chara('(', "\' (unclosed parenthesis)");
		}
		i++;
	}
	node_lexeme = ft_substr(str, 0, i);//a secur
	new_node = lstnew(node_lexeme);//a secur
	new_node->type = WORD;
	lstadd_back(tokens, new_node);
	return (i);
}

size_t op_control_token(t_lst **tokens, char *str, char op)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 0;
	while (str[i] == op && i != 2)
		i++;
	node_lexeme = ft_substr(str, 0, i);//a secur
	new_node = lstnew(node_lexeme);//a secur
	if (i == 1 && op == '|')
		new_node->type = PIPE;
	else if (i == 2 && op == '|')
		new_node->type = OR;
	else
		new_node->type = AND;
	lstadd_back(tokens, new_node);
	return (i);
}

size_t op_redirection_token(t_lst **tokens, char *str, char op)
{
	size_t	i;
	char *node_lexeme;
	t_lst	*new_node;

	i = 0;
	while (str[i] == op && i != 2)
		i++;
	if (i == 1 && op == '<' && str[i] == '>')
		i++;
	node_lexeme = ft_substr(str, 0, i);//a secur
	new_node = lstnew(node_lexeme);//a secur
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

size_t op_parenthesis(t_lst **tokens, char *str, char op)
{
	char *node_lexeme;
	t_lst	*new_node;

	node_lexeme = ft_substr(str, 0, 1);//a secur
	new_node = lstnew(node_lexeme);//a secur
	if (op == '(')
		new_node->type = PAR_OPEN;
	if (op == ')')
		new_node->type = PAR_CLOSE;
	lstadd_back(tokens, new_node);
	return (1);
}

void	create_tokens(t_lst **tokens, char *input)
{
	size_t	token_len;

	token_len = 0;
	if (*input == '\0')
		return ;
	while (*input == ' ' || *input == '\t')
		input++;
	if ((*input == '&' && *(input + 1) != '&') || *input == ';' || *input == '\\')
		not_interpret_chara(*input, "\'");
	if (ft_strchr("|&;()<> \t\'\"", *input) == NULL)
		token_len = word_token(tokens, input);
	if (*input == '\'')
		token_len = single_quote_token(tokens, input);
	if (*input == '\"')
		token_len = double_quote_token(tokens, input);
	if (*input == '(' || *input == ')')
		token_len = op_parenthesis(tokens, input, *input);
	if (*input == '|' || *input == '&')
		token_len = op_control_token(tokens, input, *input);
	if (*input == '<' || *input == '>')
		token_len = op_redirection_token(tokens, input, *input);
	create_tokens(tokens, input + token_len);
}

// int	main(void)
// {
// 	t_lst	*tokens;
// 	t_lst	*temp;
// 	char *input = "echo  ss$(ls)sss";
// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	fusion_quote_token(tokens);
// 	temp = tokens;
// 	while (temp)
// 	{
// 		printf("\nNew node\n");
// 		printf("%s\n", temp->lexeme);
// 		printf("%d\n", temp->type);
// 		temp = temp->next;
// 	}
// }
