/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/18 18:01:35 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
typedef enum e_exit_type t_exit;
typedef struct s_minishell t_minishell;
typedef enum e_strjoin t_strjoin;
# include "libft.h"
# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdlib.h>
# include "parsing.h"
# include "redirections.h"
# include "debug.h"
# include "hashmap.h"
# include "wildcards.h"
# include "environment.h"
# include "garbage_collector.h"
# include "ft_printf.h"
# include <readline/readline.h>
# include <readline/history.h>

# define PROMPT "\001\033[35m\002$> \001\033[0m\002"
# define BUILTIN_FATAL_ERROR	177
# define BUILTIN_FLAG_ECHO_NONL	'n'
# define BUILTIN_FLAG_SEPARATOR	'-'
# define BUILTIN_ERROR_EXIT	"exit: too many arguments"
# define BUILTIN_ERROR_EXPORT	"export : Not a valid identifier : "
# define BUILTIN_ERROR_CD	"cd "
# define BUILTIN_ERROR_CD_ARGS	"cd : too many arguments"
# define BUILTIN_ERROR_CD_COLON "cd : "
# define BUILTIN_ERROR_CD_NOTSET " not set\n"

struct s_minishell
{
	t_hashmap			gc;
	t_hashmap			environment;
	char				**environment_tab;
	char				*script_file;
	size_t				line;
	int					last_cmd_pid;
	int					script_fd;
	bool				script_mode;
	t_leaf 				*command_tab;
	bool				in_child;
	bool				in_pipe;
	bool				is_subshell;
	unsigned char		exit_code;
};

typedef enum e_strjoin
{
	NO_FREE = 1<<0,
	FREE_PARAM1 = 1<<1,
	FREE_PARAM2 = 1<<2,
}	t_strjoin;

typedef enum e_exit_type
{
	EXIT_NORMAL,
}	t_exit;

typedef enum e_builtin_type
{
	BUILTIN_TYPE_CD,
	BUILTIN_TYPE_EXPORT,
	BUILTIN_TYPE_ENV,
	BUILTIN_TYPE_UNSET,
	BUILTIN_TYPE_PWD,
	BUILTIN_TYPE_ECHO,
	BUILTIN_TYPE_EXIT,
	BUILTIN_TYPE_NOTBUILTIN
}	t_builtin_type;

void			parsing_exec(char *input, t_minishell *data);

void				wildcards_in_cmd(t_leaf *cmd, t_minishell *data);

void				parse_cmd(t_leaf *cmd, t_minishell *data);
t_leaf			*evaluate_ast(t_AST_node *node, t_minishell *data);
int				execute_cmd(t_leaf *cmd, t_minishell *data);

void			open_error(t_minishell *data);
void			pipe_error(t_minishell *data);
void			fork_error(t_minishell *data);
void			malloc_error(t_minishell *data);
void			exit_minishell(t_minishell *data);

void			*check_malloc(void *content, t_minishell *data);

bool			exec_builtins(char **argv, bool exit, t_minishell *data);

int				exit_builtin(int argc, char **argv, t_minishell *data);
int				env_builtin(t_minishell *data);
int				pwd_builtin();
int				export_builtin(int argc, char **argv, t_minishell *data);
int				unset_builtin(int argc, char **argv, t_minishell *data);
int				echo_builtin(int argc, char **argv);
int				cd_builtin(int argc, char **argv, t_minishell *data);

// UTILS
bool			write_str_secure(char *str, int fd);
t_envvar		*get_pwd(char *pwd_type, char *default_value, t_minishell *data);
char			*ft_strjoin_alt(char *s1, char *s2, t_strjoin free_what);
char			*ft_strjoin_alt_gc(char *s1, char *s2, t_strjoin free_what, t_minishell *data);
unsigned long	hash_ptr(void *ptr);

char			*find_command(char *cmd, t_minishell *data);
void			try_builtin(t_builtin_type type, 
					int argc, char **v, t_minishell *data);
t_builtin_type	get_builtin(char *cmd);
void			command_notfound(char *cmd, t_minishell *data);

int				check_flags_c(int argc, char **argv);
void			handle_shell(t_minishell *data);
void			handle_script(char **argv, t_minishell *data);
void			handle_cflag(char **argv, t_minishell *data);
void			exec_command(char **argv, t_minishell *data);

void			close_fds(int fd1, int fd2, int fd3, int fd4);
void			close_input_output(t_leaf *cmd);

void			wait_childs(t_minishell *data);

/*SIGNALS*/
void			setup_signals_prompt(void);;

#endif
