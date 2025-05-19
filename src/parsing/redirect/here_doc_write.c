/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_write.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 01:56:12 by val               #+#    #+#             */
/*   Updated: 2025/05/19 02:38:55 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"

bool	unclosed_par_here_doc(char *str);
char	*handle_dollars_in_here_doc(bool unclosed_par, char **input,
	t_lst *token_eof, t_minishell *data);

void	write_in_here_doc_file(t_leaf *cmd, t_lst *token_eof,
			int fd, t_minishell *data)
{
	char	*input;
	bool	unclosed_par;

	unclosed_par = false;
	while (1)
	{
		input = readline("> ");
		gc_add(input, data);
		ft_putstr_fd("LE ZIZI PROPRE", 2);
		if (!input || g_signal_status == SIGINT)
		{
			gc_free(input, data);
			ft_putstr_fd("LE ZIZI SALE", 2);
			break;
		}
		if (unclosed_par == false)
			unclosed_par = unclosed_par_here_doc(input);
		if (ft_strcmp(input, token_eof->lexeme) == 0)
		{
			if (unclosed_par == true)
			{
				ft_putstr_fd("minishell: unexpected EOF ", 2);
				ft_putendl_fd("while looking for matching `)'", 2);
				cmd->fd_input = -1;
			}
			gc_free(input, data);
			break ;
		}
		handle_dollars_in_here_doc(unclosed_par, &input, token_eof, data);
		ft_putendl_fd(input, fd);
		gc_free(input, data);
	}
}


