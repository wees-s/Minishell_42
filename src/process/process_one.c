/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:36 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/27 11:58:50 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"
#include "process.h"

static void	process_one_fork(char **line_tokens, t_shell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		exec_external(line_tokens, sh);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		sh->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_exit_status = 128 + WTERMSIG(status);
}

void	process_one_split(t_shell *sh)
{
	char	*cmd;
	char	**line_tokens;

	sh->fd_in = dup(STDIN_FILENO);
	sh->fd_out = dup(STDOUT_FILENO);
	redirect_fd(sh, 0);
	cmd = command(sh->s_pipe[0]);
	free_array(sh->s_pipe);
	sh->s_pipe = NULL;
	if (cmd == NULL)
	{
		dup2_close_in_out(sh->fd_in, sh->fd_out);
		return ;
	}
	line_tokens = tokens(cmd);
	free(cmd);
	cmd = NULL;
	if (is_builtin(line_tokens[0]))
		exec_line(line_tokens, sh, cmd);
	else
		process_one_fork(line_tokens, sh);
	free_array(line_tokens);
	dup2_close_in_out(sh->fd_in, sh->fd_out);
}
