/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:20:34 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/19 20:55:52 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include "minishell.h"

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

# define BUILTIN_NAME_CD	"cd"
# define BUILTIN_NAME_EXPORT	"export"
# define BUILTIN_NAME_ENV	"env"
# define BUILTIN_NAME_UNSET	"unset"
# define BUILTIN_NAME_PWD	"pwd"
# define BUILTIN_NAME_ECHO	"echo"
# define BUILTIN_NAME_EXIT	"exit"

# define BUILTIN_FLAG_ECHO_NONL	'n'
# define BUILTIN_FLAG_SEPARATOR	'-'
# define BUILTIN_FATAL_ERROR	177
# define BUILTIN_ERROR_TOOMANY	"too many arguments"

void			try_builtin(t_builtin_type type, 
	int argc, char **v, t_minishell *data);
t_builtin_type	get_builtin(char *cmd);

int				exit_builtin(int argc, char **argv, t_minishell *data);
int				env_builtin(t_minishell *data);
int				pwd_builtin();
int				export_builtin(int argc, char **argv, t_minishell *data);
int				unset_builtin(int argc, char **argv, t_minishell *data);
int				echo_builtin(int argc, char **argv);
int				cd_builtin(int argc, char **argv, t_minishell *data);

#endif