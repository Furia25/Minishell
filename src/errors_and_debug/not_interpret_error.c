/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   not_interpret_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:12:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/15 15:52:53 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	not_interpret_chara(char chara, char *str, t_minishell	*data)
{
	ft_printf_fd(2, "minishell: we are not supposed to manage this `%c%s\n", chara, str);
	// free data->gc
	data->exit_code = 2;
	return (EXIT_FAILURE);
}
