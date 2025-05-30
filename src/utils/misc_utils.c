/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:58:02 by vdurand           #+#    #+#             */
/*   Updated: 2025/05/30 19:03:32 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>
#include <sys/stat.h>

int	check_flags_c(int argc, char **argv)
{
	int		index;
	char	*temp;

	if (argc == 1)
		return (0);
	index = 1;
	while (argv[index])
	{
		temp = argv[index];
		if (*temp != '-')
			return (index);
		temp++;
		while (*temp && temp)
		{
			if (*temp != 'c')
				return (index);
			temp++;
		}
		index++;
	}
	return (index);
}

void	set_echoctl(bool state)
{
	struct termios	term;

	if (!isatty(STDOUT_FILENO) || !isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	if (state)
		term.c_lflag |= ECHOCTL;
	else
		term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int	get_echoctl(void)
{
	struct termios	term;

	if (!isatty(STDOUT_FILENO) || !isatty(STDIN_FILENO))
		return (-1);
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return (-1);
	return (term.c_lflag & ECHOCTL);
}

bool	is_directory(char *path)
{
	struct stat		file_data;

	ft_memset(&file_data, 0, sizeof(struct stat));
	lstat(path, &file_data);
	return (S_ISDIR(file_data.st_mode));
}

pid_t	s_fork(t_minishell *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		data->in_child = true;
	return (pid);
}
