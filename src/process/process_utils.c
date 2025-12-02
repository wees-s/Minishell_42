/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:23 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/28 16:21:45 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"
#include "process.h"

void	dup2_close_in_out(int fd_in, int fd_out)
{
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	else
		return (0);
}

void	redirect_fd(t_shell *sh, int i)
{
	int	fd[2];
	int	*fd_temp;

	fd_temp = parse_fd(sh, i);
	fd[0] = fd_temp[0];
	fd[1] = fd_temp[1];
	free(fd_temp);
	if (fd[0] != sh->fd_in)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	if (fd[1] != sh->fd_out)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

void	exec_external(char **tokens, t_shell *sh)
{
	int		i;
	int		flag;
	char	*temp;

	i = 0;
	flag = 0;
	temp = NULL;
	while (tokens[0][i])
	{
		if (tokens[0][i] == '/')
			flag = 1;
		i++;
	}
	i = 0;
	while (tokens[++i])
	{
		temp = remove_quotes_str(tokens[i], 0, 0);
		free(tokens[i]);
		tokens[i] = ft_strdup(temp);
		free(temp);
	}
	if (flag == 1)
		cmd_bar(tokens, sh);
	else
		cmd_not_bar(tokens, sh);
}

void	exec_line(char **line_tokens, t_shell *sh)
{
	if (ft_strncmp(line_tokens[0], "echo", 5) == 0)
		echo(line_tokens, sh);
	else if (ft_strncmp(line_tokens[0], "cd", 3) == 0)
		cd(line_tokens, sh);
	else if (ft_strncmp(line_tokens[0], "pwd", 4) == 0)
		pwd(sh);
	else if (ft_strncmp(line_tokens[0], "export", 7) == 0)
		export_arg(line_tokens, sh);
	else if (ft_strncmp(line_tokens[0], "unset", 6) == 0)
		unset_env(sh, line_tokens);
	else if (ft_strncmp(line_tokens[0], "env", 4) == 0)
	{
		if (line_tokens[1] == NULL)
		{
			sh->last_exit_status = 0;
			print_env(sh->env);
		}
		else
		{
			sh->last_exit_status = 127;
			printf("env: ‘%s’: No such file or directory\n", line_tokens[1]);
		}
	}
	else if (ft_strncmp(line_tokens[0], "exit", 5) == 0)
		exit_process(line_tokens, sh);
}
