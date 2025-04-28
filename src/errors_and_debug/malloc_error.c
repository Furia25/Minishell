/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:17:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/28 16:05:00 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	check_malloc(void *content, t_minishell *data)
{
	if (content == NULL)
	{
		malloc_error(data);
		return ;
	}
	gc_add(content, data);
}

void	malloc_error(t_minishell *data)
{
	ft_putstr_fd("minishell: malloc error", 2);
	data->exit_code = EXIT_FAILURE;
	exit_minishell(data);
}

void	exit_minishell(t_minishell *data)
{
	//free_lesfddanslarbrestp
	hashmap_free_content(&data->environment);
	gc_clean(data);
	exit(data->exit_code);
}
