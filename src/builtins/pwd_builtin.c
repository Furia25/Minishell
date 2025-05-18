/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:20:41 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/18 23:45:26 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin(void)
{
	char	*temp;

	temp = getcwd(NULL, 0);
	if (!temp)
	{
		perror("pwd :");
		return (BUILTIN_FATAL_ERROR);
	}
	ft_printf("%s\n", temp);
	free(temp);
	return (EXIT_SUCCESS);
}
