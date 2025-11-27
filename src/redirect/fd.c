/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:31:41 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:31:42 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"
#include "redirect.h"

static void	func_flag(char *line, int *i, int quote[2])
{
	while (line[*i] && line[*i] != '<' && line[*i] != '>')
	{
		if (line[*i] == '\'')
			quote[0] = !quote[0];
		else if (line[*i] == '\"')
			quote[1] = !quote[1];
		(*i)++;
	}
}

static t_fd	init_parse_fd(int fd_in, int fd_out)
{
	t_fd	fd;

	fd.i = 0;
	fd.tmp = 0;
	fd.fd = malloc(sizeof(int) * 2);
	if (!fd.fd)
	{
		fd.fd = NULL;
		return (fd);
	}
	fd.fd[0] = fd_in;
	fd.fd[1] = fd_out;
	fd.quote[0] = 0;
	fd.quote[1] = 0;
	return (fd);
}

int	*parse_fd(t_shell *sh, int i)
{
	t_fd	fd;

	fd = init_parse_fd(sh->fd_in, sh->fd_out);
	while (sh->s_pipe[i][fd.i])
	{
		func_flag(sh->s_pipe[i], &fd.i, fd.quote);
		if (!fd.quote[0] && !fd.quote[1])
		{
			if (sh->s_pipe[i][fd.i] == '<')
			{
				if_read(&fd, sh, i);
				if (sh->s_pipe[i][fd.i])
					continue ;
			}
			else if (sh->s_pipe[i][fd.i] == '>')
			{
				if_write(sh->s_pipe[i], sh->fd_out, &fd);
				if (sh->s_pipe[i][fd.i])
					continue ;
			}
		}
		if (sh->s_pipe[i][fd.i])
			fd.i++;
	}
	return (fd.fd);
}
