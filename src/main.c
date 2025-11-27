/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:31:20 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/27 13:39:01 by bedantas         ###   ########.fr       */
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

	line = readline("@minishell ~ ");
	if (line == NULL)
	{
		rl_clear_history();
		free_list(&sh->env);
		sh->last_exit_status = 0;
		printf("exit\n");
		exit(EXIT_SUCCESS);
	}
	if (line[0] != '\0')
		add_history(line);
	return (line);
}

static int	valid_input(char *line, t_shell *sh)
{
	t_valid	v;

	v.quote1 = 0;
	v.quote2 = 0;
	v.last_pipe = 0;
	v.found_char = 0;
	if (!valid_pipe(line, &v, 0)
		|| !valid_red(line, '>') || !valid_red(line, '<'))
	{
		printf("Syntax error\n");
		sh->last_exit_status = 2;
		return (0);
	}
	else if (!valid_quotes(line))
	{
		printf("unexpected EOF while looking for matching `\"\'\n");
		sh->last_exit_status = 2;
		return (0);
	}
	return (1);
}

static void	redirect_and_command(char *input, t_shell *sh)
{
	char	*line;
	int		count_pipe;

	count_pipe = 0;
	line = expand_arg(sh->env, input, 0);
	if (!valid_input(line, sh))
	{
		free(line);
		return ;
	}
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

	(void)argc;
	(void)argv;
	sh.env = clone_env(envp);
	sh.last_exit_status = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = read_input(&sh);
		if (input[0] != '\0')
			redirect_and_command(input, &sh);
		free(input);	
	}
	rl_clear_history();
	free_list(&sh.env);
	return (0);
}

// cat < README.md < Makefile << here
