/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_in_here_doc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:33:14 by alpayet           #+#    #+#             */
/*   Updated: 2025/05/05 17:41:51 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
size_t	skip_subshell_and_special_chara(char *str);
size_t	env_var_len(char *str);
char	*ev_result(char *str, size_t ev_len, t_minishell *data);
char	*handle_ev_in_here_doc(char *str, t_minishell *data);


static char	*ev_expand(char *str, char *ev_str, t_minishell *data)
{
	size_t	ev_len;
	char	*buff;

	ev_len = env_var_len(ev_str);
	buff = ft_strjoin_alt_gc(str, ev_result(ev_str, ev_len, data),
		FREE_PARAM2, data);
	check_malloc(buff, data);
	return (check_malloc(ft_strjoin_alt_gc(buff,
		handle_ev_in_here_doc(ev_str + ev_len, data),
		FREE_PARAM1 | FREE_PARAM2, data), data));
}

char	*handle_ev_in_here_doc(char *str, t_minishell *data)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		i += skip_subshell_and_special_chara(str + i);
		if (str[i] == '$' && str[i + 1] != '\0'
			&& str[i + 1] != '\'' &&  str[i + 1] != '\"')
		{
			str[i] = '\0';
			i++;
			return (ev_expand(str, str + i, data));
		}
		i++;
	}
	return (check_malloc(ft_substr(str, 0, ft_strlen(str)), data));
}
