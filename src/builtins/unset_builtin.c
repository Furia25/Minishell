/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:48:05 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/18 23:45:17 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_builtin(int argc, char **argv, t_minishell *data)
{
	int				exit_code;
	t_hash_entry	*entry;

	if (argc == 1)
		return (EXIT_FAILURE);
	exit_code = EXIT_SUCCESS;
	while (argc > 1)
	{
		entry = hashmap_search(hash(argv[argc - 1]), &data->environment);
		if (entry != NULL)
		{
			entry->status = TOMBSTONE;
			data->environment.charge_factor -= 1;
		}
		else
			exit_code = EXIT_FAILURE;
		argc--;
	}
	return (exit_code);
}
