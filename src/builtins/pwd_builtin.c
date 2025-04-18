/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:20:41 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/18 17:47:28 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin()
{
	char	*temp;

	temp = getcwd(NULL, 0);
	if (!temp)
	{
		perror("pwd :");
		return (EXIT_FAILURE);
	}
	ft_printf("%s", temp);
	return (EXIT_SUCCESS);
}
