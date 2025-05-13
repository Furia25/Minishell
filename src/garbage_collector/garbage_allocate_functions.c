/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_allocate_functions.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:58:23 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/14 00:00:17 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	*memset_fast(void *ptr, int value, size_t num);

void	*gc_malloc(size_t size, t_minishell *data)
{
	void	*result;

	result = malloc(size);
	check_malloc(result, data);
	return (result);
}

void	*gc_calloc(size_t count, size_t size, t_minishell *data)
{
	void	*temp;
	size_t	rsize;

	if (size == 0 || count == 0)
		return (NULL);
	if (count > __SIZE_MAX__ / size \
		|| size > __SIZE_MAX__ / count)
		return (NULL);
	rsize = count * size;
	temp = malloc(rsize);
	check_malloc(temp, data);
	memset_fast(temp, 0, rsize);
	return (temp);
}
