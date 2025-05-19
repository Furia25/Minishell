/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:47:07 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/20 01:24:59 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

static void print_spaces(size_t depth)
{
	while (depth != 0)
	{
		ft_printf_fd(2, " ");
		depth--;
	}
}

static void	print_enum(t_lexeme_type control_operator)
{
	if (control_operator == PIPE)
		ft_printf_fd(2, "PIPE\n");
	if (control_operator == OR)
		ft_printf_fd(2, "OR\n");
	if (control_operator == AND)
		ft_printf_fd(2, "AND\n");
}

static void	print_ast(t_ast_node *node_ast, size_t depth)
{
	if (node_ast->type == NODE_COMMAND)
	{
		print_spaces(depth);
		ft_printf_fd(2, "%s\n", node_ast->command->tokens->lexeme);
	}
	else
	{
		print_spaces(depth);
		print_enum(node_ast->s_ope_node.control_operator);
		print_spaces(depth);
		ft_printf_fd(2, "left_node\n");
		print_ast(node_ast->s_ope_node.left_node, depth + 2);
		print_spaces(depth);
		ft_printf_fd(2, "right_node\n");
		print_ast(node_ast->s_ope_node.right_node, depth + 2);
	}
}

void	print_debug_ast(t_ast_node *top_node_ast, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf_fd(2, "%s", str);
		print_ast(top_node_ast, 0);
	}
}
