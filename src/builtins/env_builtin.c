/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:59:32 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/14 15:42:10 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_builtin(t_minishell *data)
{
	env_print(&data->environment);
	return (EXIT_SUCCESS);
}