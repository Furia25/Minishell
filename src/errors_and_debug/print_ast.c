/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:47:07 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/22 18:22:04 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	print_lst_one_line(t_lst *lst);

static void	print_spaces(size_t depth)
{
	while (depth != 0)
	{
		ft_printf_fd(STDERR_FILENO, "  ");
		depth--;
	}
}

static void	print_enum(t_lexeme_type control_operator)
{
	if (control_operator == PIPE)
		ft_printf_fd(STDERR_FILENO, "PIPE\n");
	if (control_operator == OR)
		ft_printf_fd(STDERR_FILENO, "OR\n");
	if (control_operator == AND)
		ft_printf_fd(STDERR_FILENO, "AND\n");
}

static void	print_ast(t_ast_node *node_ast, size_t depth)
{
	if (node_ast->type == NODE_COMMAND)
	{
		print_spaces(depth);
		print_lst_one_line(node_ast->command->tokens);
	}
	else
	{
		print_spaces(depth);
		print_enum(node_ast->s_ope_node.control_operator);
		print_spaces(depth);
		ft_printf_fd(STDERR_FILENO, "left_node\n");
		print_ast(node_ast->s_ope_node.left_node, depth + 2);
		print_spaces(depth);
		ft_printf_fd(STDERR_FILENO, "right_node\n");
		print_ast(node_ast->s_ope_node.right_node, depth + 2);
	}
}

void	print_debug_ast(t_ast_node *top_node_ast, int debug, char *str)
{
	if (DEBUG == debug || DEBUG == 1)
	{
		ft_printf_fd(STDERR_FILENO, "%s", str);
		print_ast(top_node_ast, 0);
	}
}
