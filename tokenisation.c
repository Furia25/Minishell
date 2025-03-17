/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:22:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/17 16:39:23 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int word_token(t_list **tokens, char *str)
{
	int	i;
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
int single_quote_token(t_list **tokens, char *str)
{
	int	i;
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
int double_quote_token(t_list **tokens, char *str)
{
	int	i;
	t_list	*new_node;
	char	*node_content;

	i = 1;
	while (ft_isprint(str[i]) != 0 && str[i] != '\"')
		i++;
	node_content = ft_substr(str, 0, i + 1);//a secur
	new_node = ft_lstnew(node_content);//a secur
	ft_lstadd_back(tokens, new_node);
	return (i + 1);
		
}
int pipe_or_token(t_list **tokens, char *str)
{
	int	i;
	t_list	*new_node;
	char	*node_content;

	i = 0;
	while (str[i] == '|')
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
int and_token(t_list **tokens, char *str)
{
	int	i;
	t_list	*new_node;
	char	*node_content;

	i = 0;
	while (str[i] == '&')
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
	int	token_len;

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
	if (*input == '|')
		token_len = pipe_or_token(tokens, input);
	if (*input == '&')
		token_len = and_token(tokens, input);
	create_tokens(tokens, input + token_len);
}

int	main(void)
{
	t_list	*tokens;
	t_list	*temp;
	char *input = "		abcde	   	fgh	||	'ffezz'	|";


	tokens = NULL;
	create_tokens(&tokens, input);
	temp = tokens;
	while (temp)
	{
		printf("%s\n", (char*)temp->content);
		temp = temp->next;
	}
	
}
