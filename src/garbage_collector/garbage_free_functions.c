/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_free_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:59:24 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/13 23:59:54 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	gc_free(void *ptr, t_minishell *data)
{
	hashmap_remove(hash_ptr(ptr), &data->gc);
}

void	gc_free_node(t_lst *node, t_minishell *data)
{
	gc_free(node->lexeme, data);
	gc_free(node, data);
}

void	gc_free_cmd(t_leaf *cmd, t_minishell *data)
{
	t_lst *token;
	t_lst *buff_next;

	token = cmd->tokens;
	while (token != NULL)
	{
		buff_next = token->next;
		gc_free_node(token, data);
		token = buff_next;		
	}
	gc_free(cmd, data);
}

void	gc_free_ast(t_AST_node	*ast_node, t_minishell *data)
{
	t_lst *token;
	t_lst *buff_next;

	if (ast_node->type == NODE_COMMAND)
		gc_free_cmd(ast_node->command, data);
	else
	{
		gc_free_ast(ast_node->t_ope_node.left_node, data);
		gc_free_ast(ast_node->t_ope_node.right_node, data);
		gc_free(ast_node->t_ope_node.left_node, data);
		gc_free(ast_node->t_ope_node.right_node, data);
	}
	gc_free(ast_node, data);
}
