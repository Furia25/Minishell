/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_digits.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 00:54:45 by val               #+#    #+#             */
/*   Updated: 2025/05/17 00:55:16 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_count_digits(long long n)
{
	size_t				count;
	unsigned long long	nb;

	if (n == 0)
		return (1);
	if (n < 0)
		nb = -n;
	else
		nb = n;
	count = 0;
	while (nb != 0)
	{
		nb /= 10;
		count++;
	}
	return (count);
}
