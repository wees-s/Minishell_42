/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:31:20 by bedantas          #+#    #+#             */
/*   Updated: 2025/12/01 10:51:56 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utils/minishell.h"

void	handle_sigint(int sig)
{
	g_heredoc_child = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

char	*read_input(t_shell *sh)
{
	char	*line;

	line = readline("\001\033[38;2;255;0;180m\002Minishell$ \001\033[0m\002");
	if (line == NULL)
	{
		rl_clear_history();
		free_list(&sh->env);
		printf("exit\n");
		exit(sh->last_exit_status);
	}
	if (line[0] != '\0')
		add_history(line);
	return (line);
}

static void	status_signal(t_shell *sh)
{
	if (g_heredoc_child == SIGINT || g_heredoc_child == SIGQUIT)
	{
		sh->last_exit_status = g_heredoc_child + 128;
		g_heredoc_child = -1;
	}
}

static void	redirect_and_command(char *input, t_shell *sh)
{
	char	*line;
	int		count_pipe;

	count_pipe = 0;
	line = expand_arg(sh, input, 0);
	if (!valid_input(line, sh))
	{
		free(line);
		return ;
	}
	sh->fd_in = dup(STDIN_FILENO);
	sh->fd_out = dup(STDOUT_FILENO);
	sh->s_pipe = split_pipe(line, 0, 0);
	free(line);
	while (sh->s_pipe[count_pipe])
		count_pipe++;
	if (count_pipe == 1)
		process_one_split(sh);
	else
		process_pipes(sh);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	sh;

	if (argc > 1)
	{
		printf("minishell: unsupported arguments (\'%s\')\n", argv[1]);
		exit(2);
	}
	sh.env = clone_env(envp);
	sh.last_exit_status = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = read_input(&sh);
		status_signal(&sh);
		if (input[0] != '\0' && g_heredoc_child == -1)
			redirect_and_command(input, &sh);
		free(input);
	}
	rl_clear_history();
	free_list(&sh.env);
	return (0);
}
