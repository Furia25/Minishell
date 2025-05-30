/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   not_interpret_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:12:56 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/26 19:33:11 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	not_interpret_chara(char chara, char *str, t_minishell	*data)
{
	ft_printf_fd(STDERR_FILENO,
		"%s: we are not supposed to handle this `%c%s\n",
		MINISHELL_NAME, chara, str);
	data->exit_code = 2;
}
