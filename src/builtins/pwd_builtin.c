/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:20:41 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/19 19:57:11 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

int	pwd_builtin(void)
{
	char	*temp;

	temp = getcwd(NULL, 0);
	if (!temp)
	{
		print_basic_error(BUILTIN_NAME_PWD);
		return (BUILTIN_FATAL_ERROR);
	}
	ft_printf("%s\n", temp);
	free(temp);
	return (EXIT_SUCCESS);
}
