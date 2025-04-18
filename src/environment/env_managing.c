/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_managing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:05:32 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/18 17:25:56 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envvar	*new_envvar(char *key, char *value)
{
	t_envvar		*var;

	if (!key)
		return (NULL);
	if (!value)
		return (NULL);
	var = ft_calloc(1, sizeof(t_envvar));
	if (!var)
		return (NULL);
	var->name = key;
	var->value = value;
	var->exported = true;
	return (var);
}

void	envvar_free(void *el)
{
	t_envvar	*var;

	if (!el)
		return ;
	var = (t_envvar *) el;
	if (var->name)
		free(var->name);
	if (var->value)
		free(var->value);
	free(var);
}
