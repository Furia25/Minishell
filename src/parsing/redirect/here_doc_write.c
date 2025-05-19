/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_write.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 01:56:12 by val               #+#    #+#             */
/*   Updated: 2025/05/20 01:19:09 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include "minishell.h"
#include <errno.h>

char		*handle_dollars_in_here_doc(bool unclosed_par, char **input,
				t_lst *token_eof, t_minishell *data);
static int	check_null_readline(char *input, t_minishell *data);
bool		unclosed_par_here_doc(char *str);
static int	unexpected_eof(void);
static void	readline_and_check_unclosed(char **input,
				bool *unclosed_par, t_minishell *data);

static int	dummy_event(void)
{
	if (DEBUG == 42)
		ft_putstr_fd("DEBUG : THIS MESSAGE SHOULD NOT BE PRINTED\n", 2);
	return (0);
}

/**
 * @brief Écrit les lignes saisies par l'utilisateur
 * dans un fichier temporaire pour un heredoc.
 *
 * @param fd Descripteur de fichier vers lequel écrire.
 * @param token_eof Jeton représentant le délimiteur de fin du heredoc.
 * @param data Structure contenant les données du shell.
 *
 * @return

 * @retval 0 Erreur : le heredoc n'a pas été traité et ou écrit correctement.
 * @retval 1 Succès : le heredoc a été traité et écrit correctement ou EOF
 * par control D.
 * @retval 2 Interruption par signal (par exemple, SIGINT via Ctrl+C).
 */
int	write_in_here_doc_file(int fd, t_lst *token_eof, t_minishell *data)
{
	char	*input;
	int		readline_check;
	bool	unclosed_par;

	input = NULL;
	unclosed_par = false;
	rl_event_hook = dummy_event;
	while (1)
	{
		readline_and_check_unclosed(&input, &unclosed_par, data);
		readline_check = check_null_readline(input, data);
		if (readline_check != 0)
			return (readline_check);
		if (ft_strcmp(input, token_eof->lexeme) == 0)
		{
			gc_free(input, data);
			if (unclosed_par == true)
				return (unexpected_eof());
			break ;
		}
		handle_dollars_in_here_doc(unclosed_par, &input, token_eof, data);
		ft_putendl_fd(input, fd);
		gc_free(input, data);
	}
	return (1);
}

static void	readline_and_check_unclosed(char **input,
				bool *unclosed_par, t_minishell *data)
{
	*input = readline("> ");
	gc_add(*input, data);
	if (*unclosed_par == false)
		*unclosed_par = unclosed_par_here_doc(*input);
}

static int	unexpected_eof(void)
{
	ft_printf_fd(2, "%s: here-doc: \
unexpected EOF while looking for matching \')'\n",
		MINISHELL_NAME);
	return (0);
}

static int	check_null_readline(char *input, t_minishell *data)
{
	if (!input || g_signal_status == SIGINT)
	{
		gc_free(input, data);
		if (errno != 0)
			raise_error(data);
		if (g_signal_status == SIGINT)
		{
			data->exit_code = 128 + SIGINT;
			return (2);
		}
		ft_printf_fd(2, "%s: warning: here-document at line %d delimited by \
end-of-file (wanted 'EOF')\n", MINISHELL_NAME, data->line);
		return (1);
	}
	return (0);
}
