/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:42:00 by val               #+#    #+#             */
/*   Updated: 2025/04/18 02:42:18 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_home(t_minishell *data);

int	cd_builtin(int argc, char **argv, t_minishell *data)
{
	(void) argv;
	if (argc == 1)
		return (handle_home(data));
	if (ft_strcmp(argv[1], "~"))
		return (handle_home(data));
	return (EXIT_FAILURE);
}

static bool	handle_home(t_minishell *data)
{
	t_hash_entry	*entry;
	t_envvar		*var;

	entry = hashmap_search(hash(ENV_HOME), &data->environment);
	if (!entry)
	{
		write_str_secure(BUILTIN_ERROR_CD_HOME, 2);
		return (EXIT_FAILURE);
	}
	else
	{
		var = (t_envvar *) entry->value;
		if (chdir(var->value) == -1)
		{
			perror(BUILTIN_ERROR_CD);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
