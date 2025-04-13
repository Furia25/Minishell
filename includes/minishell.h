/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/14 00:02:17 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "hashmap.h"
# include <stdio.h>
# include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>

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


typedef enum s_strjoin
{
	NO_FREE = 1<<0,
	FREE_PARAM1 = 1<<1,
	FREE_PARAM2 = 1<<2,
}	t_strjoin;

typedef enum s_lexeme_type
{
	VOID,
	WORD,
	SINGLE_Q,
	DOUBLE_Q,
	RED_IN,
	RED_OUT,
	RED_OUT_A,
	HERE_DOC,
	PAR_OPEN,
	PAR_CLOSE,
	AND,
	OR,
	PIPE,
	LINE_CHANGE
}	t_lexeme_type;

typedef struct s_lst
{
	char *lexeme;
	t_lexeme_type type;
	bool metacharacter_after;
	struct s_lst *next;
}	t_lst;

typedef struct s_leaf
{
	t_lst *tokens;
	int fd_input;
	int fd_output;
	int returned_value;
	bool parenthesis;
	t_lexeme_type ope_after;
}	t_leaf;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_OPERATOR_CONTROL
}	t_node_type;

typedef struct s_AST_node
{
	t_node_type	type;
	union
	{
		t_leaf	*command;
		struct
		{
			t_lexeme_type control_operator;
			struct s_AST_node *left_node;
			struct s_AST_node *right_node;
		} t_ope_node;
	};
}	t_AST_node;

t_lst	*lstnew(char *lexeme);
void	lstadd_back(t_lst **lst, t_lst *new);
void	lstdelone(t_lst *lst, void (*del)(void*));
void	lstclear(t_lst **lst, void (*del)(void*));
void	create_tokens(t_lst **tokens, char *input);
void	fusion_quote_token(t_lst *tokens);
void	check_syntax_errors(t_lst *tokens);
t_leaf *create_cmd_tab(t_lst *tokens);
char	*ft_strjoin_alt(char *s1, char *s2, t_strjoin free_what);
void	handle_all_here_doc(t_leaf *command_tab);
t_AST_node	*create_ast(t_leaf *command_tab);
t_leaf	*evaluate_ast(t_AST_node *node);
int	execute_cmd(t_leaf *cmd);
void	rm_here_doc_files(t_leaf *command_tab);



#endif
