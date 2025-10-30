#include "../../utils/minishell.h"

t_env	*sort_list(t_env *head)
{
	t_env	*cmp;
	t_env	*exp;
	char	*temp;

	if (!head)
		return (NULL);
	exp = head;
	while (exp)
	{
		cmp = exp->next;
		while (cmp)
		{
			if (ft_strcmp(exp->key, cmp->key) > 0)
			{
				temp = exp->key;
				exp->key = cmp->key;
				cmp->key = temp;
				temp = exp->value;
				exp->value = cmp->value;
				cmp->value = temp;
			}
			cmp = cmp->next;
		}
		exp = exp->next;
	}
	return (head);
}

void	creat_print_export(t_env *new_env)
{
	t_env	*exp;

	exp = sort_list(new_env);
	while (exp)
	{
		printf("declare -x ");
		printf("%s", exp->key);
		printf("=\"");
		printf("%s", exp->value);
		printf("\"\n");
		exp = exp->next;
	}
	free_list(&exp);
}

void	export_arg(char *line, t_env *new_env)
{
	int	i;

	i = 6;
	if (line[i] == '\0')
		creat_print_export(new_env);
	else if (line[i] == ' ')
	{
		while (line[i] == ' ')
			i++;
		if (line[i] == '\0')
			creat_print_export(new_env);
		else
			put_export(line, new_env);
	}
}
