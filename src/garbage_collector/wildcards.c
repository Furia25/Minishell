/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:12:12 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/02 18:37:31 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "ft_printf.h"
#include "wildcards.h"
#include <stdint.h>

static	int	ft_strcount(char *str, char c);

t_wildcard	*wildcard_init(char *str, t_garbage_collector *gc)
{
	t_wildcard	*wildcard;
	int			asterisk;
	
	asterisk = ft_strcount(str, WILDCARD_SYMBOL);
	if (asterisk < 1)
		return (NULL);
	wildcard = gc_calloc(1, sizeof(t_wildcard), gc);
	if (!wildcard)
		return (NULL);
	wildcard->asterisk = asterisk;
	wildcard->origin = str;
	while (str[wildcard->left] && str[wildcard->left] != WILDCARD_SYMBOL)
		wildcard->left++;
	if (wildcard->asterisk == 2)
	{
		wildcard->mid = wildcard->left + 1;
		while (str[wildcard->mid] && str[wildcard->mid] != WILDCARD_SYMBOL)
			wildcard->mid++;
		wildcard->right = wildcard->mid + 1;
	}
	else
		wildcard->right = wildcard->left + 1;
	while (str[wildcard->right])
		wildcard->right++;
	return (wildcard);
}

static	int	ft_strcount(char *str, char c)
{
	int		count;
	size_t	index;

	index = 0;
	count = 0;
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}

int	main(int argc, char *argv[])
{
	t_garbage_collector	gc;
	
	gc_init(&gc);
	t_wildcard *test = wildcard_init(argv[1], &gc);
	if (!test)
	{
		ft_printf("No wilcard!\n");
		gc_clean(&gc);
		return (0);
	}
	ft_printf("LEFT : %d\n", test->left);
	ft_printf("MID : %d\n", test->mid);
	ft_printf("RIGHT : %d\n", test->right);
	gc_clean(&gc);
	return (0);
}