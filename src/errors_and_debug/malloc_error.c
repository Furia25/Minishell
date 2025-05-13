/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:17:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/13 15:30:08 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	*check_malloc(void *content, t_minishell *data)
{
	if (content == NULL)
	{
		malloc_error(data);
		return (NULL);
	}
	gc_add(content, data);
	return (content);
}

void	malloc_error(t_minishell *data)
{
	perror("minishell");
	data->exit_code = EXIT_FAILURE;
	exit_minishell(data);
}

void	exit_minishell(t_minishell *data)
{
	//free_lesfddanslarbrestp
	hashmap_free_content(&data->environment);
	free_chartab(data->environment_tab);
	if (data->script_fd != -1)
		close(data->script_fd);
	gc_clean(data);
	exit(data->exit_code);
}
