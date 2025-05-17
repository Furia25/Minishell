/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:58:23 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/17 14:00:00 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	*memset_fast(void *ptr, int value, size_t num);

void	*gc_malloc(size_t size, t_minishell *data)
{
	void	*result;

	result = malloc(size);
	check_malloc(result, data);
	return (result);
}

void	*gc_calloc(size_t count, size_t size, t_minishell *data)
{
	void	*temp;
	size_t	rsize;

	if (size == 0 || count == 0)
		return (NULL);
	if (count > __SIZE_MAX__ / size \
		|| size > __SIZE_MAX__ / count)
		return (NULL);
	rsize = count * size;
	temp = malloc(rsize);
	check_malloc(temp, data);
	memset_fast(temp, 0, rsize);
	return (temp);
}

void	gc_free(void *ptr, t_minishell *data)
{
	hashmap_remove(hash_ptr(ptr), &data->gc);
}

void	gc_free_node(t_lst *node, t_minishell *data)
{
	gc_free(node->lexeme, data);
	gc_free(node, data);
}

void	gc_free_ast(t_AST_node	*ast_node, t_minishell *data)
{
	t_lst *token;
	t_lst *buff;

	if (ast_node->type == NODE_COMMAND)
	{
		token = ast_node->command->tokens;
		while (token != NULL)
		{
			buff = token;
			token = token->next;
			gc_free_node(buff, data);
		}
	}
	if (ast_node->type != NODE_COMMAND)
	{
		gc_free_ast(ast_node->t_ope_node.left_node, data);
		gc_free_ast(ast_node->t_ope_node.right_node, data);
		gc_free(ast_node->t_ope_node.left_node, data);
		gc_free(ast_node->t_ope_node.right_node, data);
	}
	gc_free(ast_node, data);
}
