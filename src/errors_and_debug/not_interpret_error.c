/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   not_interpret_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:12:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/14 21:35:32 by alpayet          ###   ########.fr       */
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
