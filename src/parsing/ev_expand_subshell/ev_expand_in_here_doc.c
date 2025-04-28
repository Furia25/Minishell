/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_in_here_doc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:33:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/04/29 00:59:21 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
size_t	skip_subshell(char *str);
char	*ev_result(char *str, size_t ev_len, t_minishell *data);
char	*handle_ev_in_here_doc(char *str, t_minishell *data);

static size_t	env_var_len(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '$'
		&& str[i] != '\''&& str[i] != '\"')
		i++;
	return (i);
}

static char	*ev_expand(char *str, char *ev_str, t_minishell *data)
{
	size_t	ev_len;
	char	*buff;

	ev_len = env_var_len(ev_str);
	if (ev_len == 0)
		return (ft_calloc(1, sizeof(char)));
	buff = ft_strjoin_alt(str, ev_result(ev_str, ev_len, data), FREE_PARAM2);
	check_malloc(buff, data);
	return (check_malloc(ft_strjoin_alt(buff,
		handle_ev_in_here_doc(ev_str + ev_len, data),
		FREE_PARAM1 | FREE_PARAM2), data));
}

char	*handle_ev_in_here_doc(char *str, t_minishell *data)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		skip_subshell(str + i);
		if (str[i] == '$' && str[i + 1] != '(')
		{
			if (str[i + 1] != '\'' && str[i + 1] != '\"')
			{
				str[i] = '\0';
				i++;
				return (ev_expand(str, str + i, data));
			}
		}
		i++;
	}
	return (check_malloc(ft_substr(str, 0, ft_strlen(str)), data));
}
