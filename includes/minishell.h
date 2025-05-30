/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:14:45 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/30 19:09:37 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "parsing.h"
# include "minishell_structs.h"

# include "garbage_collector.h"
# include "ft_printf.h"

# ifndef ECHOCTL
#  define ECHOCTL 0001000
# endif

# ifndef MINISHELL_HARD_PATH
#  define MINISHELL_HARD_PATH	"/usr/bin:/bin"
# endif

# define MINISHELL_NAME	"minishell"

# define MINISHELL_WELCOME_START	"\001\033[1;35m\002Welcome to "
# define MINISHELL_WELCOME_END		"!\n\001\033[0m\002"
# define PROMPT "\001\033[35m\002$> \001\033[0m\002"

# define METACHARACTER	"|&;()<> \t"
# define BLANK	" \t"
# define BASIC_WHITESPACE	" \t\n"
# define HANDLE_EV_EXPAND_CASES	"_?"

typedef enum e_strjoin
{
	NO_FREE = 1<<0,
	FREE_PARAM1 = 1<<1,
	FREE_PARAM2 = 1<<2,
}	t_strjoin;

void			*check_malloc(void *content, t_minishell *data);

void			parsing_exec(char *input, t_minishell *data);

// PRINT ERRORS
void			print_basic_error(char *error_name);
void			print_extended_error(char *error_name,
					char *cause, char *error);

// ERRORS
void			command_notfound(char *cmd, t_minishell *data);
void			raise_error_category(char *error_category, t_minishell *data);
void			raise_error(t_minishell *data);
void			exit_minishell(t_minishell *data);

// UTILS
char			*ft_strjoin_alt(char *s1, char *s2, t_strjoin free_what);
char			*ft_strjoin_alt_gc(char *s1, char *s2, t_strjoin free_what,
					t_minishell *data);
unsigned long	hash_ptr(void *ptr);
void			set_echoctl(bool state);
int				get_echoctl(void);
bool			is_directory(char *path);
pid_t			s_fork(t_minishell *data);

// MAIN MANAGING
int				check_flags_c(int argc, char **argv);
void			handle_shell(t_minishell *data);
void			handle_script(char **argv, t_minishell *data);
void			handle_cflag(char **argv, t_minishell *data);

// EXEC OF ONE SIMPLE CMD
int				execute_cmd(t_leaf *cmd, t_minishell *data);

// EXEC
void			exec_command(t_leaf *cmd, char **argv, t_minishell *data);
void			secure_close(int fd);
void			secure_close_input_output(t_leaf *cmd);
void			wait_childs(t_minishell *data);
bool			exec_builtins(t_leaf *cmd,
					char **argv, bool exit, t_minishell *data);

#endif
