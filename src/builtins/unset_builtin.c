/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:48:05 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/26 15:43:20 by vdurand          ###   ########.fr       */
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
		entry = hashmap_search(hash(argv[argc]), &data->environment);
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
