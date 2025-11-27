/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:30:59 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/27 13:18:34 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/redirect/redirect.h"
# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <termios.h>

extern volatile sig_atomic_t	g_heredoc_child;

/****************************** BUILT-IN ******************************/

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	int		last_exit_status;
	char	**s_pipe;
	int		fd_in;
	int		fd_out;
}	t_shell;

//export
void		update_value(char *split_line, t_env *pointer);
void		check_to_put(char *split_line, t_env **env);
void		export_arg(char **line_tokens, t_shell *sh);
int			valid_arg(char **split_line, t_shell *sh);

void		exit_process(char **tokens, t_shell *sh, char *cmd);
void		cd(char **line, t_shell *sh);
void		echo(char **token, t_shell *sh);
void		print_env(t_env *ptr);
char		**split_env(char *line);
void		put_env(t_env **list, char *line);
t_env		*clone_env(char **envp);
t_env		*get_value_by_name(t_env **new_env, char *str);
void		pwd(t_shell *sh);
void		unset_env(t_shell *sh, char **cmd);

/****************************** EXPANDER ******************************/

char		*join_and_free(char *s1, const char *s2);
char		*expanded(t_env *begin_list, char *key);
char		*get_key(char *str);
char		*expand_arg(t_env *new_env, char *line, int i);

/****************************** PROCESS *******************************/

//exec
void		exec_access_perror(char *s, char **array, int x, t_env *env);
void		exec_access_putstr(char *s, char **array, int x, t_env *env);
char		**path(t_env *env);
char		*command_valid(char **tokens, char **path_split);
void		cmd_bar(char **tokens, t_env *env, int fd_in, int fd_out);
void		cmd_not_bar(char **tokens, t_shell *sh);
char		**env_list_to_array(t_env *env, int i, char *tmp1, char *tmp2);

//tokens
char		**split_with_quotes(char *line);
char		*remove_quotes_str(char *string, int i, int j);
char		**tokens(char *line);

char		*command(char *line);
void		process_one_split(t_shell *sh);
void		process_pipes(t_shell *sh);
void		dup2_close_in_out(int fd_in, int fd_out);
int			is_builtin(char *cmd);
void		redirect_fd(t_shell *sh, int i);
void		exec_external(char **tokens, t_shell *sh);
void		exec_line(char **line_tokens, t_shell *sh, char *cmd);

/****************************** REDIRECT ******************************/

//heredoc
char		*heredoc(t_shell *sh, int i, int *fd_malloc);
int			red_heredoc(t_shell *sh, int i, int *fd_malloc);

void		if_write(char *line, int fd_out, t_fd *fd);
void		if_read(t_fd *fd, t_shell *sh, int i);
int			*parse_fd(t_shell *sh, int i);
char		**split_pipe(char *line, int i, int j);

/**********************************************************************/

//free.c
void		free_array(char **array);
void		free_list(t_env **begin_list);

//main.c
char		*read_input(t_shell *sh);
void		handle_sigint(int sig);

//valid_input_utils.c
typedef struct s_valid
{
	int	quote1;
	int	quote2;
	int	last_pipe;
	int	found_char;
}	t_valid;

int			valid_pipe(char *line, t_valid *v, int i);
int			valid_red(char *line, char c);
int			valid_quotes(char *line);

#endif
