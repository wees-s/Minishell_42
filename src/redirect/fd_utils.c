/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:31:53 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:31:54 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"
#include "redirect.h"

static int	open_write_fd(char *line, int i)
{
	int		start;
	char	*file;
	int		fd;
	int		append;

	append = 0;
	if (line[i + 1] == '>')
	{
		append = 1;
		i += 2;
	}
	else
		i++;
	while (line[i] == ' ')
		i++;
	start = i;
	while (line[i] && !ft_isspace(line[i]) && line[i] != '<' && line[i] != '>')
		i++;
	file = ft_substr(line, start, i - start);
	if (append == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	free(file);
	return (fd);
}

void	if_write(char *line, int fd_out, t_fd *fd)
{
	fd->tmp = open_write_fd(line, fd->i);
	if (fd->tmp != -1)
	{
		if (fd->fd[1] != fd_out)
			close(fd->fd[1]);
		fd->fd[1] = fd->tmp;
	}
	if (line[fd->i] == '>' && line[fd->i + 1] == '>')
		fd->i += 2;
	else
		fd->i += 1;
}

static int	open_read_fd(char *line, int i)
{
	int		start;
	char	*file;
	int		fd;

	i++;
	while (line[i] == ' ')
		i++;
	start = i;
	while (line[i] && !ft_isspace(line[i]) && line[i] != '<' && line[i] != '>')
		i++;
	file = ft_substr(line, start, i - start);
	fd = open(file, O_RDONLY, 0444);
	free(file);
	return (fd);
}

void	if_read(t_fd *fd, t_shell *sh, int i)
{
	int	tmp;

	tmp = 0;
	if (sh->s_pipe[i][fd->i] == '<' && sh->s_pipe[i][fd->i + 1] != '<')
	{
		tmp = open_read_fd(sh->s_pipe[i], fd->i);
		if (tmp != -1)
		{
			if (fd->fd[0] != sh->fd_in)
				close(fd->fd[0]);
			fd->fd[0] = tmp;
		}
		fd->i += 1;
	}
	else if (sh->s_pipe[i][fd->i] == '<' && sh->s_pipe[i][fd->i + 1] == '<')
	{
		tmp = red_heredoc(sh, i, fd->fd);
		if (tmp != -1)
		{
			if (fd->fd[0] != sh->fd_in)
				close(fd->fd[0]);
			fd->fd[0] = tmp;
		}
		fd->i += 2;
	}
}
