/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:48:45 by vdurand           #+#    #+#             */
/*   Updated: 2025/04/02 17:04:53 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARDS_H
# include "libft.h"
# include <stddef.h>

# define WILDCARD_SYMBOL	'*'
typedef struct s_wildcard
{
	char	*origin;
	size_t	left;
	size_t	mid;
	size_t	right;
	char	**result;
	int		asterisk;
}	t_wildcard;

#endif
