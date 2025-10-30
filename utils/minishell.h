#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

//env
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env		*create_env(char **string);
t_env		*last_env(t_env *begin_list);
void		*put_env(t_env **begin_list, char *string);
t_env		*clone_env(char **string);
void		print_env(t_env *ptr);

char		*find_and_split(char *string);
char		**split_env(char *string);

//expander
char		*expand_arg(t_env *begin_list, char *string);
char		*expanded(t_env *begin_list, char *key);
char		*expand_arg(t_env *begin_list, char *string);

//export
void		check_to_put(char *split_line, t_env **new_env); //25 linhas
void		update_value(char *split_line, t_env *pointer);

t_env		*sort_list(t_env *head);
void		creat_print_export(t_env *new_env);
void		export_arg(char *line, t_env *new_env);

int			have_equal(char *str);
int			valid_arg_value(char *str);
int			valid_arg_name(char *str);
int			valid_arg(char **split_line); //25 linhas
void		put_export(char *line, t_env *new_env);

void		handle_quote_state(char c, int *in_quotes, char *quote_char);
int			count_args(char *line);
int			find_arg_end(char *line, int start);
char		**split_with_quotes(char *line);

//error_free.c
void		free_array(char **array);
void		free_list(t_env **begin_list);

//main.c
void		handle(int sig);
char		*read_input(char *ppt, t_env *new_env);
void		exec_line(char *line, t_env *new_env);
int			main(int argc, char **argv, char **envp);

//unset.c
void		unset_env(t_env **begin_list, char *string);

// collection.c
typedef struct s_collect
{
	struct s_collect	*next;
	void				*data;
}	t_collect;

t_collect	*create_item(void *data);
t_collect	*last_item(t_collect *begin_list);
t_collect	*init_collect(void);
void		close_collect(t_collect *init);
void		*put_item(t_collect **begin_list, void *data);

#endif
