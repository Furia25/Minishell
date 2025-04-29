/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/29 17:51:15 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdlib.h>
# include "parsing.h"
# include "debug.h"
# include "hashmap.h"
# include "wildcards.h"
# include "environment.h"
# include "ft_printf.h"
# include <readline/readline.h>
# include <readline/history.h>

# define PROMPT "\001\033[35m\002$> \001\033[0m\002"
# define BUILTIN_FATAL_ERROR	177
# define BUILTIN_FLAG_ECHO_NONL	'n'
# define BUILTIN_FLAG_SEPARATOR	'-'
# define BUILTIN_ERROR_EXPORT	"export : Not a valid identifier : "
# define BUILTIN_ERROR_CD	"cd "
# define BUILTIN_ERROR_CD_ARGS	"cd : too many arguments"
# define BUILTIN_ERROR_CD_COLON "cd : "
# define BUILTIN_ERROR_CD_NOTSET " not set\n"

typedef struct s_minishell
{
	t_hashmap			gc;
	t_hashmap			environment;
	unsigned char		exit_code;
}	t_minishell;

typedef enum e_strjoin
{
	NO_FREE = 1<<0,
	FREE_PARAM1 = 1<<1,
	FREE_PARAM2 = 1<<2,
}	t_strjoin;

typedef enum	e_exit_type
{
	EXIT_NORMAL,
}	t_exit;

int		not_interpret_chara(char chara, char *str, t_minishell	*data);

t_AST_node	*create_ast(t_leaf *command_tab, t_minishell *data);
t_leaf	*evaluate_ast(t_AST_node *node, t_minishell *data);
int			execute_cmd(t_leaf *cmd, t_minishell *data);

void			malloc_error(t_minishell *data);
void			exit_minishell(t_minishell *data);

void		*check_malloc(void *content, t_minishell *data);


int			env_builtin(t_minishell *data);
int			pwd_builtin();
int			export_builtin(int argc, char **argv, t_minishell *data);
int			unset_builtin(int argc, char **argv, t_minishell *data);
int			echo_builtin(int argc, char **argv);
int			cd_builtin(int argc, char **argv, t_minishell *data);

// UTILS
bool		write_str_secure(char *str, int fd);
t_envvar	*get_pwd(char *pwd_type, char *default_value, t_minishell *data);
char		*ft_strjoin_alt(char *s1, char *s2, t_strjoin free_what);
unsigned long	hash_ptr(void *ptr);

#endif
