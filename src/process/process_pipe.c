/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:30 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/28 16:47:04 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"
#include "process.h"

static void	pipes_utils(t_pipes p, t_shell *sh)
{
	int	status;

	waitpid(p.last_pid, &status, 0);
	if (WIFEXITED(status))
		sh->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_exit_status = 128 + WTERMSIG(status);
	while (wait(NULL) > 0)
		;
	free_array(sh->s_pipe);
	dup2_close_in_out(sh->fd_in, sh->fd_out);
	signal(SIGINT, handle_sigint);
}

static void	child_utils(t_shell *sh, t_pipes *p, int i)
{
	if (p->prev_fd != -1)
	{
		dup2(p->prev_fd, STDIN_FILENO);
		close(p->prev_fd);
	}
	if (sh->s_pipe[i + 1])
	{
		dup2(p->fd[1], STDOUT_FILENO);
		close(p->fd[0]);
		close(p->fd[1]);
	}
	redirect_fd(sh, i);
	p->cmd = command(sh->s_pipe[i]);
	if (p->cmd == NULL)
	{
		free_array(sh->s_pipe);
		dup2_close_in_out(sh->fd_in, sh->fd_out);
		exit(EXIT_FAILURE);
	}
	p->tokens_cmd = tokens(p->cmd);
	free(p->cmd);
}

static void	child_process(t_shell *sh, t_pipes p, int i)
{
	child_utils(sh, &p, i);
	if (!p.tokens_cmd || p.tokens_cmd[0][0] == '\0')
	{
		free_array(sh->s_pipe);
		dup2_close_in_out(sh->fd_in, sh->fd_out);
		exec_access_putstr("Command not found\n", p.tokens_cmd, 127, sh->env);
	}
	if (is_builtin(p.tokens_cmd[0]))
		exec_line(p.tokens_cmd, sh);
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_external(p.tokens_cmd, sh);
	}
	free_array(p.tokens_cmd);
	free_array(sh->s_pipe);
	free_list(&sh->env);
	dup2_close_in_out(sh->fd_in, sh->fd_out);
	exit(EXIT_SUCCESS);
}

void	process_pipes(t_shell *sh)
{
	t_pipes	p;
	int		i;

	i = 0;
	p.prev_fd = -1;
	while (sh->s_pipe[i])
	{
		if (sh->s_pipe[i + 1])
			pipe(p.fd);
		signal(SIGINT, SIG_IGN);
		p.pid = fork();
		if (p.pid == 0)
			child_process(sh, p, i);
		p.last_pid = p.pid;
		if (p.prev_fd != -1)
			close(p.prev_fd);
		if (sh->s_pipe[i + 1])
		{
			close(p.fd[1]);
			p.prev_fd = p.fd[0];
		}
		i++;
	}
	pipes_utils(p, sh);
}
