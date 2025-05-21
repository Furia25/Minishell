/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:59:32 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/21 01:00:10 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_builtin(int output, t_minishell *data)
{
	env_print(output, &data->environment, false);
	return (EXIT_SUCCESS);
}
