/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:36 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/28 16:33:15 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"
#include "process.h"

static void	process_one_fork(char **line_tokens, t_shell *sh)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		exec_external(line_tokens, sh);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_sigint);
	if (WIFEXITED(status))
		sh->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_exit_status = 128 + WTERMSIG(status);
}

static char	**trate_cmd(t_shell *sh)
{
	char	*cmd;
	char	**line_tokens;

	cmd = command(sh->s_pipe[0]);
	free_array(sh->s_pipe);
	sh->s_pipe = NULL;
	if (cmd == NULL)
	{
		dup2_close_in_out(sh->fd_in, sh->fd_out);
		return (NULL);
	}
	line_tokens = tokens(cmd);
	free(cmd);
	cmd = NULL;
	if (!line_tokens || line_tokens[0][0] == '\0')
	{
		free_array(line_tokens);
		dup2_close_in_out(sh->fd_in, sh->fd_out);
		printf("Command not found\n");
		sh->last_exit_status = 127;
		return (NULL);
	}
	return (line_tokens);
}

void	process_one_split(t_shell *sh)
{
	char	**line_tokens;

	redirect_fd(sh, 0);
	line_tokens = trate_cmd(sh);
	if (line_tokens == NULL)
		return ;
	if (is_builtin(line_tokens[0]))
		exec_line(line_tokens, sh);
	else
		process_one_fork(line_tokens, sh);
	free_array(line_tokens);
	dup2_close_in_out(sh->fd_in, sh->fd_out);
}
