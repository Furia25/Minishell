/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_expand_tilde.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:31:46 by alpayet           #+#    #+#             */
/*   Updated: 2025/06/02 14:31:52 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"

static char	*change_tilde_lexeme(t_lst *tokens, t_minishell *data);

void	expand_tilde(t_leaf *cmd, t_minishell *data)
{
	t_lst			*tokens;
	char			*new_lexeme;

	tokens = cmd->tokens;
	while (tokens)
	{
		if (tokens->type == WORD)
		{
			if ((tokens->lexeme[0] == '~' && tokens->lexeme[1] == '\0')
				|| (tokens->lexeme[0] == '~' && tokens->lexeme[1] == '/'))
			{
				new_lexeme = change_tilde_lexeme(tokens, data);
				if (new_lexeme == NULL)
				{
					tokens = tokens->next;
					continue ;
				}
			}
		}
		tokens = tokens->next;
	}
}

static char	*change_tilde_lexeme(t_lst *tokens, t_minishell *data)
{
	t_hash_entry	*search_temp;
	char			*new_lexeme;

	search_temp = hashmap_search(hash(ENV_HOME), &data->environment);
	if (search_temp == NULL)
		return (NULL);
	new_lexeme = ft_strjoin(((t_envvar *)search_temp->value)->value,
			tokens->lexeme + 1);
	check_malloc(new_lexeme, data);
	gc_free(tokens->lexeme, data);
	tokens->lexeme = new_lexeme;
	return (new_lexeme);
}
