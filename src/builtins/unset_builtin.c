/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:48:05 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/23 01:44:20 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_builtin(int argc, char **argv, t_minishell *data)
{
	unsigned long	hashed;
	int				exit_code;
	t_hash_entry	*entry;

	if (argc == 1)
		return (EXIT_FAILURE);
	exit_code = EXIT_SUCCESS;
	while (argc > 1)
	{
		hashed = hash(argv[argc - 1]);
		entry = hashmap_search(hashed, &data->environment);
		if (entry != NULL)
			hashmap_remove(hashed, &data->environment);
		else
			exit_code = EXIT_FAILURE;
		if (ft_strcmp(argv[argc - 1], "PATH") == 0)
			data->use_hard_path = false;
		argc--;
	}
	return (exit_code);
}
