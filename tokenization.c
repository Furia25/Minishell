/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:22:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/20 20:27:42 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t word_token(t_list **tokens, char *str)
{
	size_t	i;
	t_list	*new_node;
	char	*node_content;

	i = 0;
	while (ft_isalnum(str[i]) != 0)
		i++;
	node_content = ft_substr(str, 0, i);//a secur
	new_node = ft_lstnew(node_content);//a secur
	ft_lstadd_back(tokens, new_node);
	return (i);
		
}

size_t single_quote_token(t_list **tokens, char *str)
{
	size_t	i;
	t_list	*new_node;
	char	*node_content;

	i = 1;
	while (ft_isprint(str[i]) != 0 && str[i] != '\'')
		i++;
	node_content = ft_substr(str, 0, i + 1);//a secur
	new_node = ft_lstnew(node_content);//a secur
	ft_lstadd_back(tokens, new_node);
	return (i + 1);
		
}

size_t double_quote_token(t_list **tokens, char *str)
{
	size_t	i;
	t_list	*new_node;
	char	*node_content;

	i = 1;
	while (ft_isprint(str[i]) != 0 && str[i] != '\"')
		i++;
	node_content = ft_substr(str, 0, i + 1);//a secur
	new_node = ft_lstnew(node_content);//a secur
	ft_lstadd_back(tokens, new_node);
	return (i + 1);//secur sizet
		
}

size_t op_control_token(t_list **tokens, char *str, char op)
{
	size_t	i;
	t_list	*new_node;
	char	*node_content;

	i = 0;
	while (str[i] == op)
		i++;	
	if (i > 2 || (i == 1 && op == '&'))
	{
		ft_putstr_fd("error", 2);
		exit(1);
	}
	node_content = ft_substr(str, 0, i);//a secur
	new_node = ft_lstnew(node_content);//a secur
	ft_lstadd_back(tokens, new_node);
	return (i);
}

size_t op_redirection_token(t_list **tokens, char *str, char op)
{
	size_t	i;
	t_list	*new_node;
	char	*node_content;

	i = 0;
	while (str[i] == op)
		i++;	
	if (i > 2)
	{
		ft_putstr_fd("error", 2);
		exit(1);
	}
	node_content = ft_substr(str, 0, i);//a secur
	new_node = ft_lstnew(node_content);//a secur
	ft_lstadd_back(tokens, new_node);
	return (i);
}

char	*create_tokens(t_list **tokens, char *input)
{
	size_t	token_len;

	token_len = 0;
	if (*input == '\0')
		return (NULL);
	while ((*input >= 9 && *input <= 13) || *input == ' ')
		input++;
	if (ft_isalnum(*input) != 0)
		token_len = word_token(tokens,  input);
	if (*input == '\'')
		token_len = single_quote_token(tokens, input);
	if (*input == '\"')
		token_len = double_quote_token(tokens, input);
	if (*input == '|' || *input == '&')
		token_len = op_control_token(tokens, input, *input);
	if (*input == '<' || *input == '>')
		token_len = op_redirection_token(tokens, input, *input);
	create_tokens(tokens, input + token_len);
}

// int	main(void)
// {
// 	t_list	*tokens;
// 	t_list	*temp;
// 	char *input = "		abcde	   	fgh	||	'ffezz'	&& |";


// 	tokens = NULL;
// 	create_tokens(&tokens, input);
// 	temp = tokens;
// 	while (temp)
// 	{
// 		printf("%s\n", (char*)temp->content);
// 		temp = temp->next;
// 	}
	
// }
