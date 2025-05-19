/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 03:23:19 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/20 00:42:52 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef enum e_lexeme_type	t_lexeme_type;
typedef struct s_lst		t_lst;
typedef struct s_leaf		t_leaf;
typedef enum e_node_type	t_node_type;
typedef struct s_ast_node	t_ast_node;

# include "libft.h"
# include "minishell.h"

enum e_lexeme_type
{
	VOID,
	WORD,
	SINGLE_Q,
	DOUBLE_Q,
	RED_IN,
	RED_OUT,
	RED_IN_OUT,
	RED_OUT_A,
	HERE_DOC,
	PAR_OPEN,
	PAR_CLOSE,
	AND,
	OR,
	PIPE,
	DOLLAR,
	WILDCARD,
	LINE_CHANGE
};

struct s_lst
{
	char			*lexeme;
	t_lexeme_type	type;
	bool			metacharacter_after;
	struct s_lst	*next;
};

struct s_leaf
{
	t_lst			*tokens;
	int				fd_input;
	int				fd_output;
	int				returned_value;
	bool			parenthesis;
	t_lexeme_type	ope_after;
};

enum e_node_type
{
	NODE_COMMAND,
	NODE_OPERATOR_CONTROL
};

struct s_ast_node
{
	t_node_type	type;
	union
	{
		t_leaf	*command;
		struct
		{
			t_lexeme_type		control_operator;
			struct s_ast_node	*left_node;
			struct s_ast_node	*right_node;
		} s_ope_node;
	};
};

t_lst			*lstnew(char *lexeme);
t_lst			*lstlast(t_lst *lst);
void			lstadd_back(t_lst **lst, t_lst *new);
void			lstdelone(t_lst *lst, void (*del)(void*));
void			lstclear(t_lst **lst, void (*del)(void*));

bool			create_tokens(t_lst **tokens, char *input, t_minishell *data);
t_leaf			*create_cmd_tab(t_lst *tokens, t_minishell *data);
t_ast_node		*create_ast(t_leaf *command_tab, t_minishell *data);

void			ev_subshell_in_cmd(t_leaf *cmd, t_minishell *data);
void			fusion_quote_token(t_lst *tokens, t_minishell *data);
void			redirections_in_cmd(t_leaf *cmd, t_minishell *data);

char			*tokens_to_str(t_lst *tokens, t_minishell *data);
char			**tokens_to_argv(t_lst *tokens, t_minishell *data);
size_t			tab_size(char **tab);

#endif
