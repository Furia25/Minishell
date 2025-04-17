/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:06:05 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/17 18:32:04 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# define ENV_SHLVL	"SHLVL"
# define ENV_PATH	"PATH"
typedef struct s_envvar
{
	char *value;
	char *name;
}	t_envvar;

t_envvar	*new_envvar(char *key, char *value);
bool		env_populate(char **envp, t_hashmap *environment);
void		env_print(t_hashmap *environment);
void		envvar_free(void *el);
#endif