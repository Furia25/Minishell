/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/09 01:12:48 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "hashmap.h"
# include <stdio.h>
# include <stdlib.h>
# include "garbage_collector.h"
# include "wildcards.h"
# include <readline/readline.h>
# include <readline/history.h>


# define DEBUG 1
# define PROMPT "\001\033[35m\002$> \001\033[0m\002"

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_OPERATOR_CONTROL
}	t_node_type;

typedef enum s_operator
{
	VOID = 1 << 0,
	AND = 1 << 1,
	OR = 1 << 2,
	PIPE = 1 << 3,
	RED_I = 1 << 4,
	RED_O = 1 << 5,
	LINE_CHANGE = 1 << 6
}	t_operator;

typedef struct s_leaf
{
	t_list *tokens;
	char cmd_stdin;
	char cmd_stdout;
	int returned_value;
	t_operator ope_after;
}	t_leaf;

typedef struct s_AST_node
{
	t_node_type	type;
	union
	{
		t_leaf	*command;
		struct
		{
			t_operator control_operator;
			struct s_AST_node *left_node;
			struct s_AST_node *right_node;
		} t_ope_node;
	};
}	t_AST_node;


char	*create_tokens(t_list **tokens, char *input);
t_leaf *create_cmd_tab(t_list *tokens);
t_AST_node	*create_ast(t_leaf *command_tab);
t_leaf	*evaluate_ast(t_AST_node *node);

void	*memset_fast(void *ptr, int value, size_t num);

#endif
