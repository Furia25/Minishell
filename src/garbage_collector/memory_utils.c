/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:12:01 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/09 15:35:12 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdint.h>

void	*memset_fast(void *ptr, int value, size_t num)
{
	unsigned char	*p;
	unsigned char	val;
	uint64_t		val64;

	val = (unsigned char)value;
	p = ptr;
	while (((uintptr_t)p & 7) && num--)
		*p++ = val;
	val64 = (uint64_t)val | ((uint64_t)val << 8) | ((uint64_t)val << 16) \
	| ((uint64_t)val << 24) | ((uint64_t)val << 48) | ((uint64_t)val << 56);
	while (num >= 8)
	{
		*(uint64_t *)p = val64;
		p += 8;
		num -= 8;
	}
	while (num--)
		*p++ = val;
	return (ptr);
}
