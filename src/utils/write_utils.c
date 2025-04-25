/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:01:40 by val               #+#    #+#             */
/*   Updated: 2025/04/18 02:03:23 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	write_str_secure(char *str, int fd)
{
	if (fd == -1)
		return (false);
	if (!str)
		return (true);
	if (write(fd, str, ft_strlen(str)) == -1)
		return (false);
	return (true);
}
