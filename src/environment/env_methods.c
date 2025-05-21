/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:20:04 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/21 02:38:56 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	env_print_el(int output, unsigned long key,
	void *entry, bool print_local)
{
	t_envvar	*var;

	(void) key;
	var = (t_envvar *) entry;
	if (!print_local)
	{
		if (var->exported)
			ft_printf_fd(output, "%s=%s\n", var->name, var->value);
	}
	else
	{
		if (var->exported)
			ft_printf_fd(output, "declare -x %s=\"%s\"\n",
				var->name, var->value);
		else
			ft_printf_fd(output, "declare -x %s\n", var->name);
	}
}

void	env_print(int output, t_hashmap *environment, bool print_local)
{
	size_t			index;
	int				actual_count;
	t_hash_entry	entry;

	index = 0;
	actual_count = 0;
	while (index < environment->size && actual_count < environment->count)
	{
		entry = environment->table[index];
		if (entry.status == OCCUPIED)
		{
			env_print_el(output, entry.key, entry.value, print_local);
			actual_count++;
		}
		index++;
	}
}

char	**make_env(t_hashmap *env)
{
	char			**result;
	size_t			index;
	int				actual_count;

	result = ft_calloc(env->count + 1, sizeof(char *));
	if (!result)
		return (NULL);
	index = 0;
	actual_count = 0;
	while (index < env->size && actual_count < env->count)
	{
		if (env->table[index].status == OCCUPIED)
		{
			result[actual_count] = envvar_str(env->table[index].value);
			if (!result[actual_count])
			{
				free_chartab(result);
				return (NULL);
			}
			actual_count++;
		}
		index++;
	}
	return (result);
}

char	*envvar_str(t_envvar *var)
{
	char	*result;
	char	*temp;

	if (!var || !var->name || !var->value)
		return (NULL);
	temp = ft_strjoin(var->name, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin_alt(temp, var->value, FREE_PARAM1);
	if (!result)
		return (NULL);
	return (result);
}
