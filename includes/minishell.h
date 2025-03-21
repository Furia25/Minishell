/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/19 16:50:20 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "hashmap.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# define DEBUG 1
# define PROMPT "\001\033[35m\002$> \001\033[0m\002"

# define MAX_GC_ALLOCS	2048
typedef struct s_data
{
	void	*allocs[MAX_GC_ALLOCS];
}	t_data;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_OPERATOR_CONTROL
}	t_node_type;

typedef enum s_control_operator
{
	AND,
	OR,
	PIPE,
	LINE_CHANGE
}	t_control_operator;

typedef struct s_leaf
{
	char				tokens;
	char				cmd_stdin;
	char				cmd_stdout;
	int					returned_value;
	t_control_operator	ope_after;
}	t_leaf;

typedef struct s_AST_node
{
	t_node_type	type;
	union
	{
		t_leaf	*command;
		struct
		{
			int					control_operator;
			struct s_AST_node	*left_node;
			struct s_AST_node	*right_node;
		}	t_ope_node;
	};
}	t_AST_node;

#endif