/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:06:05 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/11 19:46:48 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

typedef struct s_envvar
{
	char *value;
	char *name;
}	t_envvar;

bool	env_populate(char **envp, t_hashmap *environment);
void	env_print(t_hashmap *environment);
void	envvar_free(void *el);
#endif