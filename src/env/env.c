#include "../../utils/minishell.h"

t_env	*create_env(char **string)
{
	t_env	*ptr;

	ptr = malloc(sizeof(t_env));
	if (!ptr)
		return (NULL);
	if (ptr)
	{
		ptr->key = string[0];
		ptr->value = string[1];
		ptr->next = NULL;
	}
	free(string);
	return (ptr);
}

t_env	*last_env(t_env *begin_list)
{
	while (begin_list->next != NULL)
		begin_list = begin_list->next;
	return (begin_list);
}

void	*put_env(t_env **begin_list, char *string)
{
	t_env	*last;
	t_env	*item;

	if (!begin_list)
		return (NULL);
	item = create_env(split_env(string));
	if (!item)
		return (NULL);
	if (!*begin_list)
	{
		*begin_list = item;
		return (NULL);
	}
	last = last_env(*begin_list);
	if (last)
		last->next = item;
	else
		*begin_list = item;
	return (string);
}

t_env	*clone_env(char **string)
{
	int		i;
	t_env	*ptr;

	i = 0;
	if (!string || !string[0])
		return (NULL);
	ptr = create_env(split_env(string[i]));
	i++;
	while (string[i] != NULL)
	{
		put_env(&ptr, string[i]);
		i++;
	}
	return (ptr);
}

void	print_env(t_env *ptr)
{
	while (ptr != NULL)
	{
		printf("%s=%s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
}
