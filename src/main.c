#include "../utils/minishell.h"

void	handle(int sig)
{
	if (sig == 2)
		printf("\nminishell ~ ");
}

char	*read_input(char *ppt, t_env *new_env)
{
	char		*line;

	line = readline(ppt);
	if (line == NULL)
	{
		free_list(&new_env);
		printf("exit\n");
		exit(EXIT_SUCCESS);
	}
	if (line[0] != '\0')
		add_history(line);
	return (line);
}

void	exec_line(char *line, t_env *new_env)
{
	//if (string == "echo")
	//	;
	//if (string == "cd")
	//	;
	//if (string == "pwd")
	//	;
	if (ft_strncmp(line, "export", 6) == 0)
		export_arg(line, new_env);
	if (ft_strncmp(line, "unset", 5) == 0)
		unset_env(&new_env, line);
	if (ft_strncmp(line, "env", ft_strlen(line)) == 0)
		print_env(new_env);
	//if (string == "exit")
	//	;
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*new_env;

	(void)argc;
	(void)argv;
	new_env = clone_env(envp);
	if (signal(SIGQUIT, SIG_IGN) || signal(SIGINT, handle))
		add_history(NULL);
	while (1)
	{
		line = read_input("minishell ~ ", new_env);
		if (line && line[0] != '\0')
			exec_line(line, new_env);
		if (*line)
			free(line);
		if (line == NULL)
			break ;
	}
	rl_clear_history();
	return (0);
}
