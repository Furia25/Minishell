/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:31:44 by alpayet           #+#    #+#             */
/*   Updated: 2025/03/25 03:23:14 by alpayet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/incs/get_next_line.h"

int	main(void)
{
	char	*input;
	char	*str;
	int		fd;
	fd = open("/tmp/here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	unlink("/tmp/here_doc");
	while (1)
	{
		input = readline("> ");
		if (ft_strcmp(input, "eof") == 0)
		{
			free(input);
			break ;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	fd = open("a", O_RDONLY, 0644);
	close(fd);
}