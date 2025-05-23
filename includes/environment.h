/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:06:05 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/23 03:53:59 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# define ENV_SHLVL		"SHLVL"
# define ENV_PATH		"PATH"
# define ENV_PWD		"PWD"
# define ENV_OLDPWD		"OLDPWD"
# define ENV_HOME		"HOME"

typedef struct s_envvar
{
	char	*value;
	char	*name;
	bool	exported;
}	t_envvar;

t_envvar	*new_envvar(char *key, char *value);
bool		env_populate(char **envp, t_hashmap *environment);
t_envvar	*env_instantiate(char *key, char *heap_default,
				t_minishell *data);
void		env_print(int output, t_hashmap *environment, bool print_local);
void		envvar_free(void *el);
char		*envvar_str(t_envvar *var);
char		**make_env(t_hashmap *env);
#endif