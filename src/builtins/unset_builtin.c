/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:48:05 by vdurand           #+#    #+#             */
/*   Updated: 2025/06/02 11:58:09 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_builtin(int argc, char **argv, t_minishell *data)
{
	unsigned long	hashed;
	t_hash_entry	*entry;

	if (argc == 1)
		return (EXIT_SUCCESS);
	while (argc > 1)
	{
		hashed = hash(argv[argc - 1]);
		entry = hashmap_search(hashed, &data->environment);
		if (entry != NULL)
			hashmap_remove(hashed, &data->environment);
		if (ft_strcmp(argv[argc - 1], "PATH") == 0)
			data->use_hard_path = false;
		argc--;
	}
	return (EXIT_SUCCESS);
}
