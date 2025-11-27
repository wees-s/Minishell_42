/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wedos-sa <wedos-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:00 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/27 14:14:26 by wedos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"
#include "../redirect.h"

volatile sig_atomic_t	g_heredoc_child = -1;

static void	parent_sigint(int sig)
{
	(void)sig;
	write(1, "^C\n", 2);
	if (g_heredoc_child > 0)
		kill(g_heredoc_child, SIGKILL);
	write(1, "\n", 1);
}

static void	restore_terminal(void)
{
	struct termios	t;

	ft_memset(&t, 0, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag |= ECHO;
	t.c_lflag |= ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

static void	pid_zero(t_shell *sh, int i, int fd[2], int *fd_malloc)
{
	char	*result;
	int		tty;

	if (fd_malloc[0])
		close(fd_malloc[0]);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	tty = open("/dev/tty", O_RDWR);
	dup2(tty, STDIN_FILENO);
	dup2(tty, STDOUT_FILENO);
	close(tty);
	close(fd[0]);
	result = heredoc(sh, i, fd_malloc);
	if (result)
	{
		write(fd[1], result, ft_strlen(result));
		free(result);
	}
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

int	red_heredoc(t_shell *sh, int i, int *fd_malloc)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		pid_zero(sh, i, fd, fd_malloc);
	else
	{
		g_heredoc_child = pid;
		signal(SIGINT, parent_sigint);
		close(fd[1]);
		waitpid(pid, NULL, 0);
		restore_terminal();
		g_heredoc_child = -1;
		signal(SIGINT, handle_sigint);
	}
	return (fd[0]);
}
