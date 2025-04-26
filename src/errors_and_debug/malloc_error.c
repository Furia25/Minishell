/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:17:25 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/26 02:28:05 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*check_malloc(void *content, t_minishell *data)
{
	//gc_add(content);
	if (content == NULL)
	{
		perror("minishell");
		data->exit_code = EXIT_FAILURE;
		//free_lesfddanslarbrestp
		//exit_minishell(data);
	}
	return (content);
}

// void	exit_minishell(t_minishell *data)
// {
// 	hashmap_free_content(&data->environment);
// 	gc_clean(&data->gc);
// 	exit (data->exit_code);
// }

