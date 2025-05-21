/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 03:17:03 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/21 20:23:57 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	print_debug_str(char *str, int debug, char *description_message)
{
	if (DEBUG == debug || DEBUG == 1)
		ft_printf_fd(STDERR_FILENO, "%sstr :%s\n", description_message, str);
}
