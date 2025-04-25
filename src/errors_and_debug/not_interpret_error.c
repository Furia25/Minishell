/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   not_interpret_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:12:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/24 23:30:10 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	not_interpret_chara(char chara, char *str, t_minishell	*data)
{
	ft_putstr_fd("minishell: we are not supposed to manage this `", 2);
	ft_putchar_fd(chara, 2);
	ft_putendl_fd(str, 2);
	// free data->gc
	data->exit_code = 2;
}