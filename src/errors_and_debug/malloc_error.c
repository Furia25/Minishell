/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:17:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/25 18:15:37 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_malloc(void *content, t_minishell *data)
{
	gc_add(content);
	if (content == NULL)
	{
		ft_putstr_fd("minishell: malloc error", 2);
		data->exit_code = EXIT_FAILURE;
		//free_lesfddanslarbrestp
		exit_minishell(data);
	}
}

void	exit_minishell(t_minishell *data)
{
	hashmap_free_content(&data->environment);
	gc_clean(&data->gc);
	exit (data->exit_code);
}
